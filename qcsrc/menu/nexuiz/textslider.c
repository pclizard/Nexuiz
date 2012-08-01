#ifdef INTERFACE
CLASS(NexuizTextSlider) EXTENDS(TextSlider)
	METHOD(NexuizTextSlider, configureNexuizTextSlider, void(entity, string))
	METHOD(NexuizTextSlider, setValue, void(entity, float))
	METHOD(NexuizTextSlider, configureNexuizTextSliderValues, void(entity))
	ATTRIB(NexuizTextSlider, fontSize, float, SKINFONTSIZE_NORMAL)
	ATTRIB(NexuizTextSlider, valueSpace, float, SKINWIDTH_SLIDERTEXT)
	ATTRIB(NexuizTextSlider, image, string, SKINGFX_SLIDER)
	ATTRIB(NexuizTextSlider, tolerance, vector, SKINTOLERANCE_SLIDER)
	ATTRIB(NexuizTextSlider, align, float, 0.5)
	ATTRIB(NexuizTextSlider, color, vector, SKINCOLOR_SLIDER_N)
	ATTRIB(NexuizTextSlider, colorC, vector, SKINCOLOR_SLIDER_C)
	ATTRIB(NexuizTextSlider, colorF, vector, SKINCOLOR_SLIDER_F)
	ATTRIB(NexuizTextSlider, colorD, vector, SKINCOLOR_SLIDER_D)
	ATTRIB(NexuizTextSlider, color2, vector, SKINCOLOR_SLIDER_S)

	ATTRIB(NexuizTextSlider, cvarName, string, string_null)
	METHOD(NexuizTextSlider, loadCvars, void(entity))
	METHOD(NexuizTextSlider, saveCvars, void(entity))

	ATTRIB(NexuizTextSlider, alpha, float, SKINALPHA_TEXT)
	ATTRIB(NexuizTextSlider, disabledAlpha, float, SKINALPHA_DISABLED)
ENDCLASS(NexuizTextSlider)
entity makeNexuizTextSlider(string); // note: you still need to call addValue and configureNexuizTextSliderValues!
#endif

#ifdef IMPLEMENTATION
entity makeNexuizTextSlider(string theCvar)
{
	entity me;
	me = spawnNexuizTextSlider();
	me.configureNexuizTextSlider(me, theCvar);
	return me;
}
void NexuizTextSlider_configureNexuizTextSlider(entity me, string theCvar)
{
	me.configureSliderVisuals(me, me.fontSize, me.align, me.valueSpace, me.image);
	if(theCvar)
	{
		me.cvarName = theCvar;
		me.tooltip = getZonedTooltipForIdentifier(theCvar);
		// don't load it yet
	}
}
void NexuizTextSlider_setValue(entity me, float val)
{
	if(val != me.value)
	{
		SUPER(NexuizTextSlider).setValue( me, val );
		me.saveCvars(me);
	}
}
void NexuizTextSlider_loadCvars(entity me)
{
	if not(me.cvarName)
		return;

	var float n = tokenize_console(me.cvarName);
	var string s = cvar_string(argv(0));
	float i;
	for(i = 1; i < n; ++i)
		s = strcat(s, " ", cvar_string(argv(i)));
	me.setValueFromIdentifier(me, s);
	if(me.value < 0 && n > 1)
	{
		// if it failed: check if all cvars have the same value
		// if yes, try its value as 1-word identifier
		for(i = 1; i < n; ++i)
			if(cvar_string(argv(i)) != cvar_string(argv(i-1)))
				break;
		if(i >= n)
			me.setValueFromIdentifier(me, cvar_string(argv(0)));
	}
}
void NexuizTextSlider_saveCvars(entity me)
{
	if not(me.cvarName)
		return;

	if(me.value >= 0 && me.value < me.nValues)
	{
		var float n = tokenize_console(me.cvarName);
		if(n == 1)
		{
			// this is a special case to allow spaces in the identifiers
			cvar_set(argv(0), me.getIdentifier(me));
		}
		else
		{
			float i;
			var float m = tokenize_console(strcat(me.cvarName, " ", me.getIdentifier(me)));
			if(m == n + 1)
			{
				for(i = 0; i < n; ++i)
					cvar_set(argv(i), argv(n));
			}
			else if(m == n * 2)
			{
				for(i = 0; i < n; ++i)
					cvar_set(argv(i), argv(i + n));
			}
			else
				error("NexuizTextSlider: invalid identifier ", me.getIdentifier(me), " does not match cvar list ", me.cvarName);
		}
	}
}
void NexuizTextSlider_configureNexuizTextSliderValues(entity me)
{
	me.configureTextSliderValues(me, string_null);
	me.loadCvars(me);
}
#endif
