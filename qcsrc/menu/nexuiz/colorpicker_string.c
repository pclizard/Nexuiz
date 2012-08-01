#ifdef INTERFACE
CLASS(NexuizColorpickerString) EXTENDS(Image)
	METHOD(NexuizColorpickerString, configureNexuizColorpickerString, void(entity, string, string))
	METHOD(NexuizColorpickerString, mousePress, float(entity, vector))
	METHOD(NexuizColorpickerString, mouseRelease, float(entity, vector))
	METHOD(NexuizColorpickerString, mouseDrag, float(entity, vector))
	ATTRIB(NexuizColorpickerString, cvarName, string, string_null)
	ATTRIB(NexuizColorpickerString, prevcoords, vector, '0 0 0')
	ATTRIB(NexuizColorpickerString, image, string, SKINGFX_COLORPICKER)
	ATTRIB(NexuizColorpickerString, imagemargin, vector, SKINMARGIN_COLORPICKER)
	ATTRIB(NexuizColorpickerString, focusable, float, 1)
	METHOD(NexuizColorpickerString, draw, void(entity))
	ATTRIB(NexuizColorpickerString, disabledAlpha, float, 0.3)
ENDCLASS(NexuizColorpickerString)
entity makeNexuizColorpickerString(string theCvar, string theDefaultCvar);
#endif

#ifdef IMPLEMENTATION
entity makeNexuizColorpickerString(string theCvar, string theDefaultCvar)
{
	entity me;
	me = spawnNexuizColorpickerString();
	me.configureNexuizColorpickerString(me, theCvar, theDefaultCvar);
	return me;
}

void NexuizColorpickerString_configureNexuizColorpickerString(entity me, string theCvar, string theDefaultCvar)
{
	me.cvarName = theCvar;
	me.configureImage(me, me.image);
	if(cvar_string(theCvar) != "")
		me.prevcoords = color_hslimage(stov(cvar_string(theCvar)), me.imagemargin);
	else // use default
		me.prevcoords = color_hslimage(stov(cvar_string(theDefaultCvar)), me.imagemargin);
}

float NexuizColorpickerString_mousePress(entity me, vector coords)
{
	me.mouseDrag(me, coords);
	return 1;
}

float NexuizColorpickerString_mouseDrag(entity me, vector coords)
{
	if(me.disabled)
		return 0;
	vector margin;
	margin = me.imagemargin;
	if(coords_x >= margin_x)
	if(coords_y >= margin_y)
	if(coords_x <= 1 - margin_x)
	if(coords_y <= 1 - margin_y)
	{
		me.prevcoords = coords;
	}

	return 1;
}

float NexuizColorpickerString_mouseRelease(entity me, vector coords)
{
	me.mouseDrag(me, coords);
	return 1;
}

void NexuizColorpickerString_draw(entity me)
{
	float save;
	save = draw_alpha;
	if(me.disabled)
		draw_alpha *= me.disabledAlpha;

	SUPER(NexuizColorpickerString).draw(me);

	vector sz;
	sz = draw_PictureSize(strcat(me.src, "_selected"));
	sz = globalToBoxSize(sz, draw_scale);

	if(!me.disabled)
	{
		cvar_set(me.cvarName, sprintf("%v", hslimage_color(me.prevcoords, me.imagemargin)));
		draw_Picture(me.imgOrigin + me.prevcoords - 0.5 * sz, strcat(me.src, "_selected"), sz, '1 1 1', 1);
	}
	draw_alpha = save;
}
#endif
