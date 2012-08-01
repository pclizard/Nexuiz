#ifdef INTERFACE
CLASS(NexuizColorpicker) EXTENDS(Image)
	METHOD(NexuizColorpicker, configureNexuizColorpicker, void(entity, entity))
	METHOD(NexuizColorpicker, mousePress, float(entity, vector))
	METHOD(NexuizColorpicker, mouseRelease, float(entity, vector))
	METHOD(NexuizColorpicker, mouseDrag, float(entity, vector))
	ATTRIB(NexuizColorpicker, controlledTextbox, entity, NULL)
	ATTRIB(NexuizColorpicker, image, string, SKINGFX_COLORPICKER)
	ATTRIB(NexuizColorpicker, imagemargin, vector, SKINMARGIN_COLORPICKER)
	ATTRIB(NexuizColorpicker, focusable, float, 1)
	METHOD(NexuizColorpicker, focusLeave, void(entity))
	METHOD(NexuizColorpicker, keyDown, float(entity, float, float, float))
	METHOD(NexuizColorpicker, draw, void(entity))
ENDCLASS(NexuizColorpicker)
entity makeNexuizColorpicker(entity theTextbox);
#endif

#ifdef IMPLEMENTATION
entity makeNexuizColorpicker(entity theTextbox)
{
	entity me;
	me = spawnNexuizColorpicker();
	me.configureNexuizColorpicker(me, theTextbox);
	return me;
}

void NexuizColorpicker_configureNexuizColorpicker(entity me, entity theTextbox)
{
	me.controlledTextbox = theTextbox;
	me.configureImage(me, me.image);
}

float NexuizColorpicker_mousePress(entity me, vector coords)
{
	me.mouseDrag(me, coords);
	return 1;
}

// must match hslimage.c
vector hslimage_color(vector v, vector margin)
{
    v_x = (v_x - margin_x) / (1 - 2 * margin_x);
    v_y = (v_y - margin_y) / (1 - 2 * margin_y);
    if(v_x < 0) v_x = 0;
    if(v_y < 0) v_y = 0;
    if(v_x > 1) v_x = 1;
    if(v_y > 1) v_y = 1;
    if(v_y > 0.875) // grey bar
        return hsl_to_rgb(eZ * v_x);
    else
        return hsl_to_rgb(v_x * 6 * eX + eY + v_y / 0.875 * eZ);
}

vector color_hslimage(vector v, vector margin)
{
	vector pos;
	v = rgb_to_hsl(v);
	if (v_y)
	{
		pos_x = v_x / 6;
		pos_y = v_z * 0.875;
	}
	else // grey scale
	{
		pos_x = v_z;
		pos_y = 0.875 + 0.07;
	}
	pos_x = margin_x + pos_x * (1 - 2 * margin_x);
	pos_y = margin_y + pos_y * (1 - 2 * margin_y);
	return pos;
}

float NexuizColorpicker_mouseDrag(entity me, vector coords)
{
	float i;
	for(;;)
	{
		i = me.controlledTextbox.cursorPos;
		if(i >= 2)
		{
			if(substring(me.controlledTextbox.text, i-2, 1) == "^")
				if(strstrofs("0123456789", substring(me.controlledTextbox.text, i-1, 1), 0) >= 0)
				{
					me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
					me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
					continue;
				}
		}

		if(i >= 5)
		{
			if(substring(me.controlledTextbox.text, i-5, 2) == "^x")
				if(strstrofs("0123456789abcdefABCDEF", substring(me.controlledTextbox.text, i-3, 1), 0) >= 0)
					if(strstrofs("0123456789abcdefABCDEF", substring(me.controlledTextbox.text, i-2, 1), 0) >= 0)
						if(strstrofs("0123456789abcdefABCDEF", substring(me.controlledTextbox.text, i-1, 1), 0) >= 0)
						{
							me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
							me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
							me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
							me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
							me.controlledTextbox.keyDown(me.controlledTextbox, K_BACKSPACE, 8, 0);
							continue;
						}
		}
		break;
	}

	vector margin;
	margin = me.imagemargin;
	if(coords_x >= margin_x)
	if(coords_y >= margin_y)
	if(coords_x <= 1 - margin_x)
	if(coords_y <= 1 - margin_y)
		me.controlledTextbox.enterText(me.controlledTextbox, rgb_to_hexcolor(hslimage_color(coords, margin)));

	return 1;
}

float NexuizColorpicker_mouseRelease(entity me, vector coords)
{
	me.mouseDrag(me, coords);
	return 1;
}

void NexuizColorpicker_focusLeave(entity me)
{
	me.controlledTextbox.saveCvars(me.controlledTextbox);
}
float NexuizColorpicker_keyDown(entity me, float key, float ascii, float shift)
{
	return me.controlledTextbox.keyDown(me.controlledTextbox, key, ascii, shift);
}
void NexuizColorpicker_draw(entity me)
{
	SUPER(NexuizColorpicker).draw(me);

	float B, C, aC;
	C = cvar("r_textcontrast");
	B = cvar("r_textbrightness");

	// for this to work, C/(1-B) must be in 0..1
	// B must be < 1
	// C must be < 1-B
	
	B = bound(0, B, 1);
	C = bound(0, C, 1-B);

	aC = 1 - C / (1 - B);

	draw_Picture(me.imgOrigin, strcat(me.src, "_m"), me.imgSize, '0 0 0', aC);
	draw_Picture(me.imgOrigin, strcat(me.src, "_m"), me.imgSize, me.color, B);
}
#endif
