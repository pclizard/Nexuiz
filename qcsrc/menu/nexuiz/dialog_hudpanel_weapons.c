#ifdef INTERFACE
CLASS(NexuizHUDWeaponsDialog) EXTENDS(NexuizRootDialog)
	METHOD(NexuizHUDWeaponsDialog, fill, void(entity))
	ATTRIB(NexuizHUDWeaponsDialog, title, string, "Weapons Panel")
	ATTRIB(NexuizHUDWeaponsDialog, color, vector, SKINCOLOR_DIALOG_TEAMSELECT)
	ATTRIB(NexuizHUDWeaponsDialog, intendedWidth, float, 0.4)
	ATTRIB(NexuizHUDWeaponsDialog, rows, float, 17)
	ATTRIB(NexuizHUDWeaponsDialog, columns, float, 4)
	ATTRIB(NexuizHUDWeaponsDialog, name, string, "HUDweapons")
ENDCLASS(NexuizHUDWeaponsDialog)
#endif

#ifdef IMPLEMENTATION
void NexuizHUDWeaponsDialog_fill(entity me)
{
	entity e;
	string panelname = "weapons";
	float i;

	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "hud_panel_weapons", "Enable panel"));
	me.TR(me);
		me.TD(me, 1, 1.4, e = makeNexuizTextLabel(0, "Background:"));
			me.TD(me, 1, 1.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_", panelname, "_bg"))));
				e.addValue(e, "Default", "");
				e.addValue(e, "Disable", "0");
				e.addValue(e, strzone(strcat("border_", panelname)), strzone(strcat("border_", panelname)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Color:"));
		me.TD(me, 2, 2.4, e = makeNexuizColorpickerString(strzone(strcat("hud_panel_", panelname, "_bg_color")), "hud_panel_bg_color"));
			setDependentStringNotEqual(e, strzone(strcat("hud_panel_", panelname, "_bg_color")), "");
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizCheckBoxString("", cvar_string("hud_panel_bg_color"), strzone(strcat("hud_panel_", panelname, "_bg_color")), "Use default"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Border size:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_", panelname, "_bg_border"))));
				e.addValue(e, "Default", "");
				e.addValue(e, "Disable", "0");
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i * 2, 0)), strzone(ftos(i * 2)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Alpha:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_", panelname, "_bg_alpha"))));
				e.addValue(e, "Default", "");
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i/10, 1)), strzone(ftos(i/10)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Team Color:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_", panelname, "_bg_color_team"))));
				e.addValue(e, "Default", "");
				e.addValue(e, "Disable", "0");
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i/10, 1)), strzone(ftos(i/10)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.4);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "hud_configure_teamcolorforced", "Test the team color in HUD configure mode"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Padding:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_", panelname, "_bg_padding"))));
				e.addValue(e, "Default", "");
				for(i = 0; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i - 5, 0)), strzone(ftos(i - 5)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Fade out after:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_", panelname, "_timeout"))));
				e.addValue(e, "Never", "0");
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(strcat(ftos_decimals(i, 0), "s")), strzone(ftos(i)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.4, e = makeNexuizTextLabel(0, "Fade effect:"));
		me.TD(me, 1, 0.8, e = makeNexuizRadioButton(3, "hud_panel_weapons_timeout_effect", "0", "None"));
			setDependentStringNotEqual(e, strzone(strcat("hud_panel_", panelname, "_timeout")), "0");
		me.TD(me, 1, 0.8, e = makeNexuizRadioButton(3, "hud_panel_weapons_timeout_effect", "1", "Slide"));
			setDependentStringNotEqual(e, strzone(strcat("hud_panel_", panelname, "_timeout")), "0");
		me.TD(me, 1, 0.8, e = makeNexuizRadioButton(3, "hud_panel_weapons_timeout_effect", "2", "Alpha"));
			setDependentStringNotEqual(e, strzone(strcat("hud_panel_", panelname, "_timeout")), "0");
	me.TR(me);
		me.TD(me, 1, 2, e = makeNexuizTextLabel(0, "Weapon icons:"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.4, e = makeNexuizTextLabel(0, "Show weapon ID as:"));
			me.TD(me, 1, 0.8, e = makeNexuizRadioButton(2, "hud_panel_weapons_label", "0", "None"));
			me.TD(me, 1, 0.8, e = makeNexuizRadioButton(2, "hud_panel_weapons_label", "1", "Number"));
			me.TD(me, 1, 0.8, e = makeNexuizRadioButton(2, "hud_panel_weapons_label", "2", "Bind"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 2, e = makeNexuizCheckBox(0, "hud_panel_weapons_accuracy", "Show Accuracy"));
		me.TD(me, 1, 2, e = makeNexuizCheckBox(0, "hud_panel_weapons_ammo", "Show Ammo"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Ammo bar color:"));
		me.TD(me, 2, 2.4, e = makeNexuizColorpickerString("hud_panel_weapons_ammo_color", "hud_panel_weapons_ammo_color"));
			setDependent(e, "hud_panel_weapons_ammo", 1, 1);
		me.TR(me);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.4, e = makeNexuizTextLabel(0, "Ammo bar alpha:"));
			me.TD(me, 1, 2.6, e = makeNexuizSlider(0.1, 1, 0.1, "hud_panel_weapons_ammo_alpha"));
			setDependent(e, "hud_panel_weapons_ammo", 1, 1);
}
#endif
