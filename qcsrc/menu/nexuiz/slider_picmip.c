#ifdef INTERFACE
CLASS(NexuizPicmipSlider) EXTENDS(NexuizTextSlider)
	METHOD(NexuizPicmipSlider, configureNexuizPicmipSlider, void(entity))
	METHOD(NexuizPicmipSlider, draw, void(entity))
	METHOD(NexuizPicmipSlider, autofix, void(entity))
ENDCLASS(NexuizPicmipSlider)
entity makeNexuizPicmipSlider(); // note: you still need to call addValue and configureNexuizTextSliderValues!
#endif

#ifdef IMPLEMENTATION
entity makeNexuizPicmipSlider()
{
	entity me;
	me = spawnNexuizPicmipSlider();
	me.configureNexuizPicmipSlider(me);
	return me;
}
void NexuizPicmipSlider_configureNexuizPicmipSlider(entity me)
{
	me.configureNexuizTextSlider(me, "gl_picmip");
	me.autofix(me);
}
float texmemsize()
{
	return
	(
		  2500 * pow(0.5, max(0, cvar("gl_picmip") + cvar("gl_picmip_other")))
		+ 1500 * pow(0.5, max(0, cvar("gl_picmip") + cvar("gl_picmip_world")))
	) * ((cvar("r_texture_dds_load") || cvar("gl_texturecompression")) ? 0.4 : 1.0); // TC: normalmaps 50%, other 25%, few incompressible, guessing 40% as conservative average
}
void NexuizPicmipSlider_autofix(entity me)
{
	float max_hard, max_soft;
	max_hard = cvar("sys_memsize_virtual");
	max_soft = cvar("sys_memsize_physical");
	if(max_hard > 0)
	{
		while(me.value > 0 && texmemsize() > max_hard)
			me.setValue(me, me.value - 1);
	}
	// TODO also check the soft limit!
	// TODO better handling than clamping the slider!
}
void NexuizPicmipSlider_draw(entity me)
{
	me.autofix(me);
	SUPER(NexuizPicmipSlider).draw(me);
}
#endif
