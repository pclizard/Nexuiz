#ifdef REGISTER_WEAPON
REGISTER_WEAPON(CRYLINK, w_crylink, IT_CELLS, 6, WEP_FLAG_NORMAL | WEP_TYPE_SPLASH, BOT_PICKUP_RATING_MID, "crylink", "crylink", "Crylink");
#else
#ifdef SVQC
.float gravity;
.float crylink_waitrelease;
.entity crylink_lastgroup;

.entity queuenext;
.entity queueprev;

void W_Crylink_CheckLinks(entity e)
{
	float i;
	entity p;

	if(e == world)
		error("W_Crylink_CheckLinks: entity is world");

	p = e;
	for(i = 0; i < 1000; ++i)
	{
		if(p.queuenext.queueprev != p || p.queueprev.queuenext != p)
			error("W_Crylink_CheckLinks: queue is inconsistent");
		p = p.queuenext;
		if(p == e)
			break;
	}
	if(i >= 1000)
		error("W_Crylink_CheckLinks: infinite chain");
}

void W_Crylink_Dequeue_Raw(entity own, entity prev, entity me, entity next)
{
	if(me == own.crylink_lastgroup)
		own.crylink_lastgroup = ((me == next) ? world : next);
	prev.queuenext = next;
	next.queueprev = prev;
}

void W_Crylink_Dequeue(entity e)
{
	W_Crylink_Dequeue_Raw(e.realowner, e.queueprev, e, e.queuenext);
}

// force projectile to explode
void W_Crylink_LinkExplode (entity e, entity e2)
{
	float a;
	a = bound(0, 1 - (time - e.fade_time) * e.fade_rate, 1);

	if(e == e.realowner.crylink_lastgroup)
		e.realowner.crylink_lastgroup = world;

	RadiusDamage (e, e.realowner, autocvar_g_balance_crylink_primary_damage * a, autocvar_g_balance_crylink_primary_edgedamage * a, autocvar_g_balance_crylink_primary_radius, world, autocvar_g_balance_crylink_primary_force * a, e.projectiledeathtype, other);

	if(e.queuenext != e2)
		W_Crylink_LinkExplode(e.queuenext, e2);

	remove (e);
}

// adjust towards center
// returns the origin where they will meet... and the time till the meeting is
// stored in w_crylink_linkjoin_time.
// could possibly network this origin and time, and display a special particle
// effect when projectiles meet there :P
// jspeed: MINIMUM jing speed
// jtime: MAXIMUM jing time (0: none)
float w_crylink_linkjoin_time;
vector W_Crylink_LinkJoin(entity e, float jspeed, float jtime)
{
	vector avg_origin, avg_velocity;
	vector targ_origin;
	float avg_dist, n;
	entity p;

	// FIXME remove this debug code
	W_Crylink_CheckLinks(e);

	w_crylink_linkjoin_time = 0;

	avg_origin = e.origin;
	avg_velocity = e.velocity;
	n = 1;
	for(p = e; (p = p.queuenext) != e; )
	{
		avg_origin += WarpZone_RefSys_TransformOrigin(p, e, p.origin);
		avg_velocity += WarpZone_RefSys_TransformVelocity(p, e, p.velocity);
		++n;
	}
	avg_origin *= (1.0 / n);
	avg_velocity *= (1.0 / n);

	if(n < 2)
		return avg_origin; // nothing to do

	// yes, mathematically we can do this in ONE step, but beware of 32bit floats...
	avg_dist = pow(vlen(e.origin - avg_origin), 2);
	for(p = e; (p = p.queuenext) != e; )
		avg_dist += pow(vlen(WarpZone_RefSys_TransformOrigin(p, e, p.origin) - avg_origin), 2);
	avg_dist *= (1.0 / n);
	avg_dist = sqrt(avg_dist);

	if(avg_dist == 0)
		return avg_origin; // no change needed

	if(jspeed == 0 && jtime == 0)
	{
		e.velocity = avg_velocity;
		UpdateCSQCProjectile(e);
		for(p = e; (p = p.queuenext) != e; )
		{
			p.velocity = WarpZone_RefSys_TransformVelocity(e, p, avg_velocity);
			UpdateCSQCProjectile(p);
		}
	}
	else
	{
		if(jtime)
		{
			if(jspeed)
				w_crylink_linkjoin_time = min(jtime, avg_dist / jspeed);
			else
				w_crylink_linkjoin_time = jtime;
		}
		else
			w_crylink_linkjoin_time = avg_dist / jspeed;
		targ_origin = avg_origin + w_crylink_linkjoin_time * avg_velocity;

		e.velocity = (targ_origin - e.origin) * (1.0 / w_crylink_linkjoin_time);
		UpdateCSQCProjectile(e);
		for(p = e; (p = p.queuenext) != e; )
		{
			p.velocity = WarpZone_RefSys_TransformVelocity(e, p, (targ_origin - WarpZone_RefSys_TransformOrigin(p, e, p.origin)) * (1.0 / w_crylink_linkjoin_time));
			UpdateCSQCProjectile(p);
		}

		// analysis:
		//   jspeed -> +infinity:
		//      w_crylink_linkjoin_time -> +0
		//      targ_origin -> avg_origin
		//      p->velocity -> HUEG towards center
		//   jspeed -> 0:
		//      w_crylink_linkjoin_time -> +/- infinity
		//      targ_origin -> avg_velocity * +/- infinity
		//      p->velocity -> avg_velocity
		//   jspeed -> -infinity:
		//      w_crylink_linkjoin_time -> -0
		//      targ_origin -> avg_origin
		//      p->velocity -> HUEG away from center
	}

	return targ_origin;
}

void W_Crylink_LinkJoinEffect_Think()
{
	// is there at least 2 projectiles very close?
	entity e, p;
	float n;
	e = self.owner.crylink_lastgroup;
	n = 0;
	if(e)
	{
		if(vlen(e.origin - self.origin) < vlen(e.velocity) * frametime)
			++n;
		for(p = e; (p = p.queuenext) != e; )
		{
			if(vlen(p.origin - self.origin) < vlen(p.velocity) * frametime)
				++n;
		}
		if(n >= 2)
		{
			if(e.projectiledeathtype & HITTYPE_SECONDARY)
			{
				if(autocvar_g_balance_crylink_secondary_joinexplode)
				{
					n = n / autocvar_g_balance_crylink_secondary_shots;
					RadiusDamage (e, e.realowner, autocvar_g_balance_crylink_secondary_joinexplode_damage * n,
									autocvar_g_balance_crylink_secondary_joinexplode_edgedamage * n,
									autocvar_g_balance_crylink_secondary_joinexplode_radius * n, e.realowner,
									autocvar_g_balance_crylink_secondary_joinexplode_force * n, e.projectiledeathtype, other);

					pointparticles(particleeffectnum("crylink_joinexplode"), self.origin, '0 0 0', n);
				}
			}
			else
			{
				if(autocvar_g_balance_crylink_primary_joinexplode)
				{
					n = n / autocvar_g_balance_crylink_primary_shots;
					RadiusDamage (e, e.realowner, autocvar_g_balance_crylink_primary_joinexplode_damage * n,
									autocvar_g_balance_crylink_primary_joinexplode_edgedamage * n,
									autocvar_g_balance_crylink_primary_joinexplode_radius * n, e.realowner,
									autocvar_g_balance_crylink_primary_joinexplode_force * n, e.projectiledeathtype, other);

					pointparticles(particleeffectnum("crylink_joinexplode"), self.origin, '0 0 0', n);
				}
			}
		}
	}
	remove(self);
}


// NO bounce protection, as bounces are limited!
void W_Crylink_Touch (void)
{
	float finalhit;
	float f;
	//PROJECTILE_TOUCH;
	local entity savenext, saveprev, saveown;
	saveown = self.realowner;
	savenext = self.queuenext;
	saveprev = self.queueprev;
	if(WarpZone_Projectile_Touch())
	{
		if(wasfreed(self))
			W_Crylink_Dequeue_Raw(saveown, saveprev, self, savenext);
		return;
	}

	float a;
	a = bound(0, 1 - (time - self.fade_time) * self.fade_rate, 1);

	finalhit = ((self.cnt <= 0) || (other.takedamage != DAMAGE_NO));
	if(finalhit)
		f = 1;
	else
		f = autocvar_g_balance_crylink_primary_bouncedamagefactor;
	if(a)
		f *= a;
	if (RadiusDamage (self, self.realowner, autocvar_g_balance_crylink_primary_damage * f, autocvar_g_balance_crylink_primary_edgedamage * f, autocvar_g_balance_crylink_primary_radius, world, autocvar_g_balance_crylink_primary_force * f, self.projectiledeathtype, other) && autocvar_g_balance_crylink_primary_linkexplode)
	{
		if(self == self.realowner.crylink_lastgroup)
			self.realowner.crylink_lastgroup = world;
		W_Crylink_LinkExplode(self.queuenext, self);
		remove (self);
		return;
	}
	else if(finalhit)
	{
		// just unlink
		W_Crylink_Dequeue(self);
		remove(self);
		return;
	}
	self.cnt = self.cnt - 1;
	self.angles = vectoangles(self.velocity);
	self.owner = world;
	self.projectiledeathtype |= HITTYPE_BOUNCE;
	// commented out as it causes a little hitch...
	//if(proj.cnt == 0)
	//	CSQCProjectile(proj, TRUE, PROJECTILE_CRYLINK, TRUE);
}

void W_Crylink_Touch2 (void)
{
	float finalhit;
	float f;
	//PROJECTILE_TOUCH;
	local entity savenext, saveprev, saveown;
	savenext = self.queuenext;
	saveprev = self.queueprev;
	saveown = self.realowner;
	if(WarpZone_Projectile_Touch())
	{
		if(wasfreed(self))
			W_Crylink_Dequeue_Raw(saveown, saveprev, self, savenext);
		return;
	}

	float a;
	a = 1 - (time - self.fade_time) * self.fade_rate;

	finalhit = ((self.cnt <= 0) || (other.takedamage != DAMAGE_NO));
	if(finalhit)
		f = 1;
	else
		f = autocvar_g_balance_crylink_secondary_bouncedamagefactor;
	if(a)
		f *= a;
	if (RadiusDamage (self, self.realowner, autocvar_g_balance_crylink_secondary_damage * f, autocvar_g_balance_crylink_secondary_edgedamage * f, autocvar_g_balance_crylink_secondary_radius, world, autocvar_g_balance_crylink_secondary_force * f, self.projectiledeathtype, other) && autocvar_g_balance_crylink_secondary_linkexplode)
	{
		if(self == self.realowner.crylink_lastgroup)
			self.realowner.crylink_lastgroup = world;
		W_Crylink_LinkExplode(self.queuenext, self);
		remove (self);
		return;
	}
	else if(finalhit)
	{
		// just unlink
		W_Crylink_Dequeue(self);
		remove(self);
		return;
	}
	self.cnt = self.cnt - 1;
	self.angles = vectoangles(self.velocity);
	self.owner = world;
	self.projectiledeathtype |= HITTYPE_BOUNCE;
	// commented out as it causes a little hitch...
	//if(proj.cnt == 0)
	//	CSQCProjectile(proj, TRUE, PROJECTILE_CRYLINK, TRUE);
}

void W_Crylink_Fadethink (void)
{
	W_Crylink_Dequeue(self);
	remove(self);
}

void W_Crylink_Attack (void)
{
	local float counter, shots;
	local entity proj, prevproj, firstproj;
	local vector s;
	vector forward, right, up;
	float maxdmg;

	if not(self.items & IT_UNLIMITED_WEAPON_AMMO)
		self.ammo_cells = self.ammo_cells - autocvar_g_balance_crylink_primary_ammo;

	maxdmg = autocvar_g_balance_crylink_primary_damage*autocvar_g_balance_crylink_primary_shots;
	maxdmg *= 1 + autocvar_g_balance_crylink_primary_bouncedamagefactor * autocvar_g_balance_crylink_primary_bounces;
	if(autocvar_g_balance_crylink_primary_joinexplode)
		maxdmg += autocvar_g_balance_crylink_primary_joinexplode_damage;

	W_SetupShot (self, FALSE, 2, "weapons/crylink_fire.wav", CHAN_WEAPON, maxdmg);
	forward = v_forward;
	right = v_right;
	up = v_up;

	shots = autocvar_g_balance_crylink_primary_shots;
	pointparticles(particleeffectnum("crylink_muzzleflash"), w_shotorg, w_shotdir * 1000, shots);
	proj = world;
	while (counter < shots)
	{
		proj = spawn ();
		proj.realowner = proj.owner = self;
		proj.classname = "spike";
		proj.bot_dodge = TRUE;
		proj.bot_dodgerating = autocvar_g_balance_crylink_primary_damage;
		if(shots == 1) {
			proj.queuenext = proj;
			proj.queueprev = proj;
		}
		else if(counter == 0) { // first projectile, store in firstproj for now
			firstproj = proj;
		}
		else if(counter == shots - 1) { // last projectile, link up with first projectile
			prevproj.queuenext = proj;
			firstproj.queueprev = proj;
			proj.queuenext = firstproj;
			proj.queueprev = prevproj;
		}
		else { // else link up with previous projectile
			prevproj.queuenext = proj;
			proj.queueprev = prevproj;
		}

		prevproj = proj;

		proj.movetype = MOVETYPE_BOUNCEMISSILE;
		PROJECTILE_MAKETRIGGER(proj);
		proj.projectiledeathtype = WEP_CRYLINK;
		//proj.gravity = 0.001;

		setorigin (proj, w_shotorg);
		setsize(proj, '0 0 0', '0 0 0');


		s = '0 0 0';
		if (counter == 0)
			s = '0 0 0';
		else
		{
			makevectors('0 360 0' * (0.75 + (counter - 0.5) / (shots - 1)));
			s_y = v_forward_x;
			s_z = v_forward_y;
		}
		s = s * autocvar_g_balance_crylink_primary_spread * g_weaponspreadfactor;
		W_SetupProjectileVelocityEx(proj, w_shotdir + right * s_y + up * s_z, v_up, autocvar_g_balance_crylink_primary_speed, 0, 0, 0, FALSE);
		proj.touch = W_Crylink_Touch;

		proj.think = W_Crylink_Fadethink;
		if(counter == 0)
		{
			proj.fade_time = time + autocvar_g_balance_crylink_primary_middle_lifetime;
			proj.fade_rate = 1 / autocvar_g_balance_crylink_primary_middle_fadetime;
			proj.nextthink = time + autocvar_g_balance_crylink_primary_middle_lifetime + autocvar_g_balance_crylink_primary_middle_fadetime;
		}
		else
		{
			proj.fade_time = time + autocvar_g_balance_crylink_primary_other_lifetime;
			proj.fade_rate = 1 / autocvar_g_balance_crylink_primary_other_fadetime;
			proj.nextthink = time + autocvar_g_balance_crylink_primary_other_lifetime + autocvar_g_balance_crylink_primary_other_fadetime;
		}
		proj.cnt = autocvar_g_balance_crylink_primary_bounces;
		//proj.scale = 1 + 1 * proj.cnt;

		proj.angles = vectoangles (proj.velocity);

		//proj.glow_size = 20;

		proj.flags = FL_PROJECTILE;

		CSQCProjectile(proj, TRUE, (proj.cnt ? PROJECTILE_CRYLINK_BOUNCING : PROJECTILE_CRYLINK), TRUE);

		other = proj; MUTATOR_CALLHOOK(EditProjectile);

		counter = counter + 1;
	}
	self.crylink_lastgroup = proj;
}

void W_Crylink_Attack2 (void)
{
	local float counter, shots;
	local entity proj, prevproj, firstproj;
	float maxdmg;

	if not(self.items & IT_UNLIMITED_WEAPON_AMMO)
		self.ammo_cells = self.ammo_cells - autocvar_g_balance_crylink_secondary_ammo;

	maxdmg = autocvar_g_balance_crylink_secondary_damage*autocvar_g_balance_crylink_secondary_shots;
	maxdmg *= 1 + autocvar_g_balance_crylink_secondary_bouncedamagefactor * autocvar_g_balance_crylink_secondary_bounces;
	if(autocvar_g_balance_crylink_secondary_joinexplode)
		maxdmg += autocvar_g_balance_crylink_secondary_joinexplode_damage;

	W_SetupShot (self, FALSE, 2, "weapons/crylink_fire2.wav", CHAN_WEAPON, maxdmg);

	shots = autocvar_g_balance_crylink_secondary_shots;
	pointparticles(particleeffectnum("crylink_muzzleflash"), w_shotorg, w_shotdir * 1000, shots);
	proj = world;
	while (counter < shots)
	{
		proj = spawn ();
		proj.realowner = proj.owner = self;
		proj.classname = "spike";
		proj.bot_dodge = TRUE;
		proj.bot_dodgerating = autocvar_g_balance_crylink_secondary_damage;
		if(shots == 1) {
			proj.queuenext = proj;
			proj.queueprev = proj;
		}
		else if(counter == 0) { // first projectile, store in firstproj for now
			firstproj = proj;
		}
		else if(counter == shots - 1) { // last projectile, link up with first projectile
			prevproj.queuenext = proj;
			firstproj.queueprev = proj;
			proj.queuenext = firstproj;
			proj.queueprev = prevproj;
		}
		else { // else link up with previous projectile
			prevproj.queuenext = proj;
			proj.queueprev = prevproj;
		}

		prevproj = proj;

		proj.movetype = MOVETYPE_BOUNCEMISSILE;
		PROJECTILE_MAKETRIGGER(proj);
		proj.projectiledeathtype = WEP_CRYLINK | HITTYPE_SECONDARY;
		//proj.gravity = 0.001;

		setorigin (proj, w_shotorg);
		setsize(proj, '0 0 0', '0 0 0');

		W_SetupProjectileVelocityEx(proj, (w_shotdir + (((counter + 0.5) / shots) * 2 - 1) * v_right * autocvar_g_balance_crylink_secondary_spread * g_weaponspreadfactor), v_up, autocvar_g_balance_crylink_secondary_speed, 0, 0, 0, FALSE);
		proj.touch = W_Crylink_Touch2;
		proj.think = W_Crylink_Fadethink;
		if(counter == (shots - 1) / 2)
		{
			proj.fade_time = time + autocvar_g_balance_crylink_secondary_middle_lifetime;
			proj.fade_rate = 1 / autocvar_g_balance_crylink_secondary_middle_fadetime;
			proj.nextthink = time + autocvar_g_balance_crylink_secondary_middle_lifetime + autocvar_g_balance_crylink_secondary_middle_fadetime;
		}
		else
		{
			proj.fade_time = time + autocvar_g_balance_crylink_secondary_line_lifetime;
			proj.fade_rate = 1 / autocvar_g_balance_crylink_secondary_line_fadetime;
			proj.nextthink = time + autocvar_g_balance_crylink_secondary_line_lifetime + autocvar_g_balance_crylink_secondary_line_fadetime;
		}
		proj.cnt = autocvar_g_balance_crylink_secondary_bounces;
		//proj.scale = 1 + 1 * proj.cnt;

		proj.angles = vectoangles (proj.velocity);

		//proj.glow_size = 20;

		proj.flags = FL_PROJECTILE;

		CSQCProjectile(proj, TRUE, (proj.cnt ? PROJECTILE_CRYLINK_BOUNCING : PROJECTILE_CRYLINK), TRUE);

		other = proj; MUTATOR_CALLHOOK(EditProjectile);

		counter = counter + 1;
	}
	self.crylink_lastgroup = proj;
}

void spawnfunc_weapon_crylink (void)
{
	weapon_defaultspawnfunc(WEP_CRYLINK);
}

float w_crylink(float req)
{
	if (req == WR_AIM)
	{
		if (random() < 0.10)
			self.BUTTON_ATCK = bot_aim(autocvar_g_balance_crylink_primary_speed, 0, autocvar_g_balance_crylink_primary_middle_lifetime, FALSE);
		else
			self.BUTTON_ATCK2 = bot_aim(autocvar_g_balance_crylink_secondary_speed, 0, autocvar_g_balance_crylink_secondary_middle_lifetime, FALSE);
	}
	else if (req == WR_THINK)
	{
		if (self.BUTTON_ATCK)
		{
			if (!self.crylink_waitrelease)
			if (weapon_prepareattack(0, autocvar_g_balance_crylink_primary_refire))
			{
				W_Crylink_Attack();
				weapon_thinkf(WFRAME_FIRE1, autocvar_g_balance_crylink_primary_animtime, w_ready);
				if(autocvar_g_balance_crylink_primary_joinspread != 0 || autocvar_g_balance_crylink_primary_jointime != 0)
					self.crylink_waitrelease = 1;
			}
		}
		else if(self.BUTTON_ATCK2 && autocvar_g_balance_crylink_secondary)
		{
			if (!self.crylink_waitrelease)
			if (weapon_prepareattack(1, autocvar_g_balance_crylink_secondary_refire))
			{
				W_Crylink_Attack2();
				weapon_thinkf(WFRAME_FIRE2, autocvar_g_balance_crylink_secondary_animtime, w_ready);
				if(autocvar_g_balance_crylink_secondary_joinspread != 0 || autocvar_g_balance_crylink_secondary_jointime != 0)
					self.crylink_waitrelease = 2;
			}
		}
		else
		{
			if (self.crylink_waitrelease)
			{
				// fired and released now!
				if(self.crylink_lastgroup)
				{
					vector pos;
					entity linkjoineffect;


					if(self.crylink_waitrelease == 1)
					{
						pos = W_Crylink_LinkJoin(self.crylink_lastgroup, autocvar_g_balance_crylink_primary_joinspread * autocvar_g_balance_crylink_primary_speed, autocvar_g_balance_crylink_primary_jointime);

					}
					else
					{
						pos = W_Crylink_LinkJoin(self.crylink_lastgroup, autocvar_g_balance_crylink_secondary_joinspread * autocvar_g_balance_crylink_secondary_speed, autocvar_g_balance_crylink_secondary_jointime);
					}

					linkjoineffect = spawn();
					linkjoineffect.think = W_Crylink_LinkJoinEffect_Think;
					linkjoineffect.classname = "linkjoineffect";
					linkjoineffect.nextthink = time + w_crylink_linkjoin_time;
					linkjoineffect.owner = self;
					setorigin(linkjoineffect, pos);


				}
				self.crylink_waitrelease = 0;
				if(!w_crylink(WR_CHECKAMMO1) && !w_crylink(WR_CHECKAMMO2))
				{
					// ran out of ammo!
					self.cnt = WEP_CRYLINK;
					self.switchweapon = w_getbestweapon(self);
				}
			}
		}
	}
	else if (req == WR_PRECACHE)
	{
		precache_model ("models/weapons/g_crylink.md3");
		precache_model ("models/weapons/v_crylink.md3");
		precache_model ("models/weapons/h_crylink.iqm");
		precache_sound ("weapons/crylink_fire.wav");
		precache_sound ("weapons/crylink_fire2.wav");
		precache_sound ("weapons/crylink_linkjoin.wav");
	}
	else if (req == WR_SETUP)
		weapon_setup(WEP_CRYLINK);
	else if (req == WR_CHECKAMMO1)
	{
		// don't "run out of ammo" and switch weapons while waiting for release
		if(self.crylink_lastgroup && self.crylink_waitrelease)
			return TRUE;
		return self.ammo_cells >= autocvar_g_balance_crylink_primary_ammo;
	}
	else if (req == WR_CHECKAMMO2)
	{
		// don't "run out of ammo" and switch weapons while waiting for release
		if(self.crylink_lastgroup && self.crylink_waitrelease)
			return TRUE;
		return self.ammo_cells >= autocvar_g_balance_crylink_secondary_ammo;
	}
	return TRUE;
};
#endif
#ifdef CSQC
float w_crylink(float req)
{
	if(req == WR_IMPACTEFFECT)
	{
		vector org2;
		org2 = w_org + w_backoff * 2;
		if(w_deathtype & HITTYPE_SECONDARY)
		{
			pointparticles(particleeffectnum("crylink_impact"), org2, '0 0 0', 1);
			if(!w_issilent)
				sound(self, CHAN_PROJECTILE, "weapons/crylink_impact2.wav", VOL_BASE, ATTN_NORM);
		}
		else
		{
			pointparticles(particleeffectnum("crylink_impactbig"), org2, '0 0 0', 1);
			if(!w_issilent)
				sound(self, CHAN_PROJECTILE, "weapons/crylink_impact.wav", VOL_BASE, ATTN_NORM);
		}
	}
	else if(req == WR_PRECACHE)
	{
		precache_sound("weapons/crylink_impact2.wav");
		precache_sound("weapons/crylink_impact.wav");
	}
	else if (req == WR_SUICIDEMESSAGE)
	{
		w_deathtypestring = "%s succeeded at self-destructing themself with the Crylink";
	}
	else if (req == WR_KILLMESSAGE)
	{
		if(w_deathtype & HITTYPE_BOUNCE)
			w_deathtypestring = "%s could not hide from %s's Crylink"; // unchecked: SPLASH (SECONDARY can't be)
		else if(w_deathtype & HITTYPE_SPLASH)
			w_deathtypestring = "%s was too close to %s's Crylink"; // unchecked: SECONDARY
		else
			w_deathtypestring = "%s took a close look at %s's Crylink"; // unchecked: SECONDARY
	}
	return TRUE;
}
#endif
#endif
