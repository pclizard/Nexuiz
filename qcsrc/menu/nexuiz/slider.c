#ifdef INTERFACE
CLASS(NexuizSlider) EXTENDS(Slider)
	METHOD(NexuizSlider, configureNexuizSlider, void(entity, float, float, float, string))
	METHOD(NexuizSlider, setValue, void(entity, float))
	ATTRIB(NexuizSlider, fontSize, float, SKINFONTSIZE_NORMAL)
	ATTRIB(NexuizSlider, valueSpace, float, SKINWIDTH_SLIDERTEXT)
	ATTRIB(NexuizSlider, image, string, SKINGFX_SLIDER)
	ATTRIB(NexuizSlider, tolerance, vector, SKINTOLERANCE_SLIDER)
	ATTRIB(NexuizSlider, align, float, 0.5)
	ATTRIB(NexuizSlider, color, vector, SKINCOLOR_SLIDER_N)
	ATTRIB(NexuizSlider, colorC, vector, SKINCOLOR_SLIDER_C)
	ATTRIB(NexuizSlider, colorF, vector, SKINCOLOR_SLIDER_F)
	ATTRIB(NexuizSlider, colorD, vector, SKINCOLOR_SLIDER_D)
	ATTRIB(NexuizSlider, color2, vector, SKINCOLOR_SLIDER_S)

	ATTRIB(NexuizSlider, cvarName, string, string_null)
	METHOD(NexuizSlider, loadCvars, void(entity))
	METHOD(NexuizSlider, saveCvars, void(entity))

	ATTRIB(NexuizSlider, alpha, float, SKINALPHA_TEXT)
	ATTRIB(NexuizSlider, disabledAlpha, float, SKINALPHA_DISABLED)
ENDCLASS(NexuizSlider)
entity makeNexuizSlider(float, float, float, string);
#endif

#ifdef IMPLEMENTATION
entity makeNexuizSlider(float theValueMin, float theValueMax, float theValueStep, string theCvar)
{
	entity me;
	me = spawnNexuizSlider();
	me.configureNexuizSlider(me, theValueMin, theValueMax, theValueStep, theCvar);
	return me;
}
void NexuizSlider_configureNexuizSlider(entity me, float theValueMin, float theValueMax, float theValueStep, string theCvar)
{
	float v, vk, vp;
	v = theValueMin;
	vk = theValueStep;
	vp = theValueStep * 10;
	while(fabs(vp) < fabs(theValueMax - theValueMin) / 40)
		vp *= 10;
	me.configureSliderVisuals(me, me.fontSize, me.align, me.valueSpace, me.image);
	me.configureSliderValues(me, theValueMin, v, theValueMax, theValueStep, vk, vp);
	if(theCvar)
	{
		me.cvarName = theCvar;
		me.loadCvars(me);
		if(tooltipdb >= 0)
			me.tooltip = getZonedTooltipForIdentifier(theCvar);
	}
}
void NexuizSlider_setValue(entity me, float val)
{
	if(val != me.value)
	{
		SUPER(NexuizSlider).setValue( me, val );
		me.saveCvars(me);
	}
}
void NexuizSlider_loadCvars(entity me)
{
	if not(me.cvarName)
		return;

	me.setValue( me, cvar(me.cvarName) );
}
void NexuizSlider_saveCvars(entity me)
{
	if not(me.cvarName)
		return;

	cvar_set(me.cvarName, ftos(me.value));
}
#endif
