#ifdef INTERFACE
CLASS(NexuizCrosshairButton) EXTENDS(RadioButton)
	METHOD(NexuizCrosshairButton, configureNexuizCrosshairButton, void(entity, float, float))
	METHOD(NexuizCrosshairButton, setChecked, void(entity, float))
	METHOD(NexuizCrosshairButton, draw, void(entity))
	ATTRIB(NexuizCrosshairButton, fontSize, float, SKINFONTSIZE_NORMAL)
	ATTRIB(NexuizCrosshairButton, image, string, SKINGFX_CROSSHAIRBUTTON)

	ATTRIB(NexuizCrosshairButton, useDownAsChecked, float, 1)
	ATTRIB(NexuizCrosshairButton, src3, string, string_null)
	ATTRIB(NexuizCrosshairButton, src4, string, string_null)

	ATTRIB(NexuizCrosshairButton, cvarName, string, string_null)
	ATTRIB(NexuizCrosshairButton, cvarValueFloat, float, 0)
	METHOD(NexuizCrosshairButton, loadCvars, void(entity))
	METHOD(NexuizCrosshairButton, saveCvars, void(entity))
ENDCLASS(NexuizCrosshairButton)
entity makeNexuizCrosshairButton(float, float);
#endif

#ifdef IMPLEMENTATION
entity makeNexuizCrosshairButton(float theGroup, float theCrosshair)
{
	entity me;
	me = spawnNexuizCrosshairButton();
	me.configureNexuizCrosshairButton(me, theGroup, theCrosshair);
	return me;
}
void NexuizCrosshairButton_configureNexuizCrosshairButton(entity me, float theGroup, float theCrosshair)
{
	me.cvarName = "crosshair";
	me.cvarValueFloat = theCrosshair;
	me.loadCvars(me);
	me.configureRadioButton(me, string_null, me.fontSize, me.image, theGroup, 0);
	me.srcMulti = 1;
	if(me.cvarValueFloat == -1)
		me.src3 = strzone(strcat("/gfx/crosshair", cvar("crosshair")));
	else
		me.src3 = strzone(strcat("/gfx/crosshair", ftos(me.cvarValueFloat)));
	me.src4 = "/gfx/crosshairdot";
}
void NexuizCrosshairButton_setChecked(entity me, float val)
{
	if(me.cvarValueFloat != -1) // preview shouldn't work as a button
	if(val != me.checked)
	{
		me.checked = val;
		me.saveCvars(me);
	}
}
void NexuizCrosshairButton_loadCvars(entity me)
{
	if not(me.cvarName)
		return;

	me.checked = (cvar(me.cvarName) == me.cvarValueFloat);
}
void NexuizCrosshairButton_saveCvars(entity me)
{
	if not(me.cvarName)
		return;

	if(me.checked)
		cvar_set(me.cvarName, ftos(me.cvarValueFloat));
	// TODO on an apply button, read _cl_color and execute the color command for it
}
void NexuizCrosshairButton_draw(entity me)
{
	vector sz, rgb;
	float a;

	rgb = stov(cvar_string("crosshair_color"));
	a = cvar("crosshair_alpha");

	if(!me.checked && !me.focused && me.cvarValueFloat != -1)
	{
		a *= me.disabledAlpha;
		rgb = '1 1 1';
	}

	if(me.cvarValueFloat == -1) // update the preview if this is the preview button
	{
		if(me.src3)
			strunzone(me.src3);
		me.src3 = strzone(strcat("/gfx/crosshair", cvar_string("crosshair")));
		me.focused = 1;
		me.checked = 0;
	}

	SUPER(NexuizCrosshairButton).draw(me);

	sz = draw_PictureSize(me.src3);
	sz = globalToBoxSize(sz, draw_scale);
	if(me.cvarValueFloat == -1)
	{
		sz = (6 * '1 1 0' + sz * cvar("crosshair_size")) * 0.08; // (6 * '1 1 0' + ...) * 0.08 here to make visible size changes happen also at bigger sizes
		if(sz_x > 0.95)
			sz = sz * (0.95 / sz_x);
		if(sz_y > 0.95)
			sz = sz * (0.95 / sz_y);
	}
	else // show the crosshair picker at full size
		sz = '0.95 0.95 0';

	draw_Picture('0.5 0.5 0' - 0.5 * sz, me.src3, sz, rgb, a);
	if(cvar("crosshair_dot"))
    {
        if(cvar_string("crosshair_dot_color") != "0")
            rgb = stov(cvar_string("crosshair_dot_color"));
		draw_Picture('0.5 0.5 0' - 0.5 * sz * cvar("crosshair_dot_size"), me.src4, sz * cvar("crosshair_dot_size"), rgb, a * cvar("crosshair_dot_alpha"));
    }
}
#endif
