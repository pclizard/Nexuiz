#ifdef REGISTER_WEAPON
REGISTER_WEAPON(UZI, w_uzi, IT_NAILS, 3, WEP_FLAG_NORMAL | WEP_TYPE_HITSCAN, BOT_PICKUP_RATING_MID, "uzi", "uzi", "Machine Gun");
#else
#ifdef SVQC
.entity muzzle_flash;

// leilei's fancy muzzleflash stuff
void Uzi_Flash_Go() 
{	
	self.frame = self.frame + 2;
	self.scale = self.scale * 0.5;
	self.alpha = self.alpha - 0.25;
	self.nextthink = time + 0.05;

	if (self.alpha <= 0)
	{
		self.think = SUB_Remove;
		self.nextthink = time;
		self.owner.muzzle_flash = world;
		return;
	}
	
}

void UziFlash()
{	
	if (self.muzzle_flash == world)
		self.muzzle_flash = spawn();	
	
	// muzzle flash for 1st person view
	setmodel(self.muzzle_flash, "models/uziflash.md3"); // precision set below
	
	self.muzzle_flash.scale = 0.75;
	self.muzzle_flash.think = Uzi_Flash_Go;
	self.muzzle_flash.nextthink = time + 0.02;
	self.muzzle_flash.frame = 2;
	self.muzzle_flash.alpha = 0.75;
	self.muzzle_flash.angles_z = random() * 180;
	self.muzzle_flash.effects = EF_ADDITIVE | EF_FULLBRIGHT | EF_LOWPRECISION;
	self.muzzle_flash.owner = self;
}

.float uzi_bulletcounter;
void W_Uzi_Attack (float deathtype)
{
	if not(self.items & IT_UNLIMITED_WEAPON_AMMO)
	{
		if (self.uzi_bulletcounter == 1)
			self.ammo_nails = self.ammo_nails - autocvar_g_balance_uzi_first_ammo;
		else
			self.ammo_nails = self.ammo_nails - autocvar_g_balance_uzi_sustained_ammo;
	}
	W_SetupShot (self, autocvar_g_antilag_bullets && autocvar_g_balance_uzi_speed >= autocvar_g_antilag_bullets, 0, "weapons/uzi_fire.wav", CHAN_WEAPON, ((self.uzi_bulletcounter == 1) ? autocvar_g_balance_uzi_first_damage : autocvar_g_balance_uzi_sustained_damage));
	if (!g_norecoil)
	{
		self.punchangle_x = random () - 0.5;
		self.punchangle_y = random () - 0.5;
	}

	// this attack_finished just enforces a cooldown at the end of a burst
	ATTACK_FINISHED(self) = time + autocvar_g_balance_uzi_first_refire * W_WeaponRateFactor();

	if (self.uzi_bulletcounter == 1)
		fireBallisticBullet(w_shotorg, w_shotdir, autocvar_g_balance_uzi_first_spread, autocvar_g_balance_uzi_speed, 5, autocvar_g_balance_uzi_first_damage, 0, autocvar_g_balance_uzi_first_force, deathtype, 0, 1, autocvar_g_balance_uzi_bulletconstant);
	else
		fireBallisticBullet(w_shotorg, w_shotdir, autocvar_g_balance_uzi_sustained_spread, autocvar_g_balance_uzi_speed, 5, autocvar_g_balance_uzi_sustained_damage, 0, autocvar_g_balance_uzi_sustained_force, deathtype, 0, 1, autocvar_g_balance_uzi_bulletconstant);
	endFireBallisticBullet();

	pointparticles(particleeffectnum("uzi_muzzleflash"), w_shotorg, w_shotdir * 1000, 1);

	UziFlash();
	W_AttachToShotorg(self.muzzle_flash, '5 0 0');

	// casing code
	if (autocvar_g_casings >= 2)
		SpawnCasing (((random () * 50 + 50) * v_right) - (v_forward * (random () * 25 + 25)) - ((random () * 5 - 70) * v_up), 2, vectoangles(v_forward),'0 250 0', 100, 3, self);
}

// weapon frames
void uzi_fire1_02()
{
	if(self.weapon != self.switchweapon) // abort immediately if switching
	{
		w_ready();
		return;
	}
	if (self.BUTTON_ATCK)
	{
		if (!weapon_action(self.weapon, WR_CHECKAMMO2))
		{
			W_SwitchWeapon_Force(self, w_getbestweapon(self));
			w_ready();
			return;
		}
		self.uzi_bulletcounter = self.uzi_bulletcounter + 1;
		W_Uzi_Attack(WEP_UZI);
		weapon_thinkf(WFRAME_FIRE1, autocvar_g_balance_uzi_sustained_refire, uzi_fire1_02);
	}
	else
		weapon_thinkf(WFRAME_FIRE1, autocvar_g_balance_uzi_sustained_refire, w_ready);
}


void uzi_mode1_fire_auto()
{
	float uzi_spread;
	
	if (self.BUTTON_ATCK)
		weapon_thinkf(WFRAME_FIRE1, autocvar_g_balance_uzi_sustained_refire, uzi_mode1_fire_auto);
	else
	{
		ATTACK_FINISHED(self) = time + autocvar_g_balance_uzi_first_refire * W_WeaponRateFactor();
		weapon_thinkf(WFRAME_FIRE1, autocvar_g_balance_uzi_sustained_refire, w_ready);
		return;
	}

	if (!weapon_action(self.weapon, WR_CHECKAMMO1))
	{
		W_SwitchWeapon_Force(self, w_getbestweapon(self));
		w_ready();
		return;
	}
	
	W_SetupShot (self, autocvar_g_antilag_bullets && autocvar_g_balance_uzi_speed >= autocvar_g_antilag_bullets, 0, "weapons/uzi_fire.wav", CHAN_WEAPON, autocvar_g_balance_uzi_sustained_damage);
	if (!g_norecoil)
	{
		self.punchangle_x = random () - 0.5;
		self.punchangle_y = random () - 0.5;
	}
	
	uzi_spread = bound(autocvar_g_balance_uzi_spread_min, autocvar_g_balance_uzi_spread_min + (autocvar_g_balance_uzi_spread_add * self.uzi_bulletcounter), autocvar_g_balance_uzi_spread_max);
	fireBallisticBullet(w_shotorg, w_shotdir, uzi_spread, autocvar_g_balance_uzi_speed, 5, autocvar_g_balance_uzi_sustained_damage, 0, autocvar_g_balance_uzi_sustained_force, WEP_UZI, 0, 1, autocvar_g_balance_uzi_bulletconstant);
	endFireBallisticBullet();
	
	self.uzi_bulletcounter = self.uzi_bulletcounter + 1;
	
	pointparticles(particleeffectnum("uzi_muzzleflash"), w_shotorg, w_shotdir * 1000, 1);

	UziFlash();
	W_AttachToShotorg(self.muzzle_flash, '5 0 0');
	
	if (autocvar_g_casings >= 2) // casing code
		SpawnCasing (((random () * 50 + 50) * v_right) - (v_forward * (random () * 25 + 25)) - ((random () * 5 - 70) * v_up), 2, vectoangles(v_forward),'0 250 0', 100, 3, self);
	
	if not(self.items & IT_UNLIMITED_WEAPON_AMMO)	
		self.ammo_nails = self.ammo_nails - autocvar_g_balance_uzi_sustained_ammo;
	
}

void uzi_mode1_fire_burst()
{
	W_SetupShot (self, autocvar_g_antilag_bullets && autocvar_g_balance_uzi_speed >= autocvar_g_antilag_bullets, 0, "weapons/uzi_fire.wav", CHAN_WEAPON, autocvar_g_balance_uzi_sustained_damage);
	if (!g_norecoil)
	{
		self.punchangle_x = random () - 0.5;
		self.punchangle_y = random () - 0.5;
	}
	
	fireBallisticBullet(w_shotorg, w_shotdir, autocvar_g_balance_uzi_burst_spread, autocvar_g_balance_uzi_speed, 5, autocvar_g_balance_uzi_sustained_damage, 0, autocvar_g_balance_uzi_sustained_force, WEP_UZI, 0, 1, autocvar_g_balance_uzi_bulletconstant);
	endFireBallisticBullet();
	
	
	pointparticles(particleeffectnum("uzi_muzzleflash"), w_shotorg, w_shotdir * 1000, 1);

	UziFlash();
	W_AttachToShotorg(self.muzzle_flash, '5 0 0');
	
	if (autocvar_g_casings >= 2) // casing code
		SpawnCasing (((random () * 50 + 50) * v_right) - (v_forward * (random () * 25 + 25)) - ((random () * 5 - 70) * v_up), 2, vectoangles(v_forward),'0 250 0', 100, 3, self);

	self.uzi_bulletcounter = self.uzi_bulletcounter + 1;
	if (self.uzi_bulletcounter == 0)
		weapon_thinkf(WFRAME_FIRE2, autocvar_g_balance_uzi_burst_refire2, w_ready);
	else
	{
		weapon_thinkf(WFRAME_FIRE2, autocvar_g_balance_uzi_burst_refire, uzi_mode1_fire_burst);
		dprint("bullets:", ftos(self.uzi_bulletcounter),"\n");
	}
		
}

void spawnfunc_weapon_machinegun(); // defined in t_items.qc

float w_uzi(float req)
{
	if (req == WR_AIM)
		if(vlen(self.origin-self.enemy.origin) < 3000 - bound(0, skill, 10) * 200)
			self.BUTTON_ATCK = bot_aim(1000000, 0, 0.001, FALSE);
		else
		{
			self.BUTTON_ATCK2 = bot_aim(1000000, 0, 0.001, FALSE);
		}
	else if (req == WR_THINK)
	{
		if(autocvar_g_balance_uzi_mode == 1)
		{
			if (self.BUTTON_ATCK)
			if (weapon_prepareattack(0, 0))
			{				
				self.uzi_bulletcounter = 0;
				uzi_mode1_fire_auto();
			}
			
			if(self.BUTTON_ATCK2)
			if(weapon_prepareattack(1, 0))
			{
				if (!weapon_action(self.weapon, WR_CHECKAMMO2))
				{
					W_SwitchWeapon_Force(self, w_getbestweapon(self));
					w_ready();
					return FALSE;
				}
				
				if not(self.items & IT_UNLIMITED_WEAPON_AMMO)	
					self.ammo_nails = self.ammo_nails - autocvar_g_balance_uzi_burst_ammo;

				self.uzi_bulletcounter = autocvar_g_balance_uzi_burst * -1;
				uzi_mode1_fire_burst();
			}
		}
		else
		{
			
			if (self.BUTTON_ATCK)
			if (weapon_prepareattack(0, 0))
			{
				self.uzi_bulletcounter = 1;
				W_Uzi_Attack(WEP_UZI); // sets attack_finished
				weapon_thinkf(WFRAME_FIRE1, autocvar_g_balance_uzi_sustained_refire, uzi_fire1_02);
			}

			if (self.BUTTON_ATCK2 && autocvar_g_balance_uzi_first)
			if (weapon_prepareattack(1, 0))
			{
				self.uzi_bulletcounter = 1;
				W_Uzi_Attack(WEP_UZI | HITTYPE_SECONDARY); // sets attack_finished
				weapon_thinkf(WFRAME_FIRE2, autocvar_g_balance_uzi_first_refire, w_ready);
			}
		}
	}
	else if (req == WR_PRECACHE)
	{
		precache_model ("models/uziflash.md3");
		precache_model ("models/weapons/g_uzi.md3");
		precache_model ("models/weapons/v_uzi.md3");
		precache_model ("models/weapons/h_uzi.iqm");
		precache_sound ("weapons/uzi_fire.wav");
	}
	else if (req == WR_SETUP)
		weapon_setup(WEP_UZI);
	else if (req == WR_CHECKAMMO1)
		if(autocvar_g_balance_uzi_mode == 1)
			return self.ammo_nails >= autocvar_g_balance_uzi_sustained_ammo;
		else
			return self.ammo_nails >= autocvar_g_balance_uzi_first_ammo;
	else if (req == WR_CHECKAMMO2)
		if(autocvar_g_balance_uzi_mode == 1)
			return self.ammo_nails >= autocvar_g_balance_uzi_burst_ammo;
		else
			return self.ammo_nails >= autocvar_g_balance_uzi_first_ammo;
	return TRUE;
};
#endif
#ifdef CSQC
float w_uzi(float req)
{
	if(req == WR_IMPACTEFFECT)
	{
		vector org2;
		org2 = w_org + w_backoff * 2;
		pointparticles(particleeffectnum("machinegun_impact"), org2, w_backoff * 1000, 1);
		if(!w_issilent)
			if(w_random < 0.05)
				sound(self, CHAN_PROJECTILE, "weapons/ric1.wav", VOL_BASE, ATTN_NORM);
			else if(w_random < 0.1)
				sound(self, CHAN_PROJECTILE, "weapons/ric2.wav", VOL_BASE, ATTN_NORM);
			else if(w_random < 0.2)
				sound(self, CHAN_PROJECTILE, "weapons/ric3.wav", VOL_BASE, ATTN_NORM);
	}
	else if(req == WR_PRECACHE)
	{
		precache_sound("weapons/ric1.wav");
		precache_sound("weapons/ric2.wav");
		precache_sound("weapons/ric3.wav");
	}
	else if (req == WR_SUICIDEMESSAGE)
		w_deathtypestring = "%s did the impossible";
	else if (req == WR_KILLMESSAGE)
	{
		if(w_deathtype & HITTYPE_SECONDARY)
			w_deathtypestring = "%s was sniped by %s";
		else
			w_deathtypestring = "%s was riddled full of holes by %s";
	}
	return TRUE;
}
#endif
#endif
