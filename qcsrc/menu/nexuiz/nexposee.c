#ifdef INTERFACE
CLASS(NexuizNexposee) EXTENDS(Nexposee)
	METHOD(NexuizNexposee, configureNexuizNexposee, void(entity))
	METHOD(NexuizNexposee, close, void(entity))
ENDCLASS(NexuizNexposee)
entity makeNexuizNexposee();
#endif

#ifdef IMPLEMENTATION
entity makeNexuizNexposee()
{
	entity me;
	me = spawnNexuizNexposee();
	me.configureNexuizNexposee(me);
	return me;
}

void NexuizNexposee_configureNexuizNexposee(entity me)
{
}

void NexuizNexposee_close(entity me)
{
	m_goto(string_null); // hide
}
#endif
