.entity accuracy;
.float accuracy_hit[WEP_MAXCOUNT];
.float accuracy_fired[WEP_MAXCOUNT];
FTEQCC_YOU_SUCK_THIS_IS_NOT_UNREFERENCED(accuracy_hit);
FTEQCC_YOU_SUCK_THIS_IS_NOT_UNREFERENCED(accuracy_fired);

float accuracy_byte(float n, float d)
{
	//print(sprintf("accuracy: %d / %d\n", n, d));
	if(n <= 0)
		return 0;
	if(n > d)
		return 255;
	return 1 + rint(n * 100.0 / d);
}

float accuracy_send(entity to, float sf)
{
	float w, f;
	entity a;
	WriteByte(MSG_ENTITY, ENT_CLIENT_ACCURACY);

	a = self.owner;
	if(a.classname == "spectator")
		a = a.enemy;
	a = a.accuracy;

	if(to != a.owner)
		if not(self.owner.cvar_cl_accuracy_data_share && autocvar_sv_accuracy_data_share)
			sf = 0;
	// note: zero sendflags can never be sent... so we can use that to say that we send no accuracy!
	WriteInt24_t(MSG_ENTITY, sf);
	if(sf == 0)
		return TRUE;
	// note: we know that client and server agree about SendFlags...
	for(w = 0, f = 1; w <= WEP_LAST - WEP_FIRST; ++w, f *= 2)
	{
		if(sf & f)
			WriteByte(MSG_ENTITY, accuracy_byte(self.(accuracy_hit[w]), self.(accuracy_fired[w])));
	}
	return TRUE;
}

// init/free
void accuracy_init(entity e)
{
	e.accuracy = spawn();
	e.accuracy.owner = e;
	e.accuracy.classname = "accuracy";
	e.accuracy.drawonlytoclient = e;
	Net_LinkEntity(e.accuracy, FALSE, 0, accuracy_send);
}

void accuracy_free(entity e)
{
	remove(e.accuracy);
}

// force a resend of a player's accuracy stats
void accuracy_resend(entity e)
{
	e.accuracy.SendFlags = 0xFFFFFF;
}

// update accuracy stats
void accuracy_set(entity e, float w, float fired, float hit)
{
	entity a;
	float b;
	a = e.accuracy;
	if(!a)
		return;
	w -= WEP_FIRST;
	b = accuracy_byte(a.(accuracy_hit[w]), a.(accuracy_fired[w]));
	a.(accuracy_hit[w]) = hit;
	a.(accuracy_fired[w]) = fired;
	if(b == accuracy_byte(hit, fired))
		return;
	w = pow(2, w);
	a.SendFlags |= w;
	FOR_EACH_CLIENT(a)
		if(a.classname == "spectator")
			if(a.enemy == e)
				a.SendFlags |= w;
}

void accuracy_add(entity e, float w, float fired, float hit)
{
	entity a;
	float b;
	a = e.accuracy;
	if(!a || !(hit || fired))
		return;
	w -= WEP_FIRST;
	b = accuracy_byte(a.(accuracy_hit[w]), a.(accuracy_fired[w]));
	if(hit)
		a.(accuracy_hit[w]) += hit;
	if(fired)
		a.(accuracy_fired[w]) += fired;
	if(b == accuracy_byte(a.(accuracy_hit[w]), a.(accuracy_fired[w])))
		return;
	w = pow(2, w);
	a.SendFlags |= w;
	FOR_EACH_CLIENT(a)
		if(a.classname == "spectator")
			if(a.enemy == e)
				a.SendFlags |= w;
}

float accuracy_isgooddamage(entity attacker, entity targ)
{
	if(!inWarmupStage)
	if(targ.flags & FL_CLIENT)
	if(targ.deadflag == DEAD_NO)
	if(IsDifferentTeam(attacker, targ))
		return TRUE;
	return FALSE;
}

float accuracy_canbegooddamage(entity attacker)
{
	if(!inWarmupStage)
		return TRUE;
	return FALSE;
}
