#ifdef INTERFACE
CLASS(NexuizHUDHealthArmorDialog) EXTENDS(NexuizRootDialog)
	METHOD(NexuizHUDHealthArmorDialog, fill, void(entity))
	ATTRIB(NexuizHUDHealthArmorDialog, title, string, "Health/Armor Panel")
	ATTRIB(NexuizHUDHealthArmorDialog, color, vector, SKINCOLOR_DIALOG_TEAMSELECT)
	ATTRIB(NexuizHUDHealthArmorDialog, intendedWidth, float, 0.4)
	ATTRIB(NexuizHUDHealthArmorDialog, rows, float, 15)
	ATTRIB(NexuizHUDHealthArmorDialog, columns, float, 4)
	ATTRIB(NexuizHUDHealthArmorDialog, name, string, "HUDhealtharmor")
ENDCLASS(NexuizHUDHealthArmorDialog)
#endif

#ifdef IMPLEMENTATION
void NexuizHUDHealthArmorDialog_fill(entity me)
{
	entity e;
	string panelname = "healtharmor";
	float i;

	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "hud_panel_healtharmor", "Enable panel"));
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
		me.TD(me, 1, 1.2, e = makeNexuizCheckBoxString("", "1 1 1", strzone(strcat("hud_panel_", panelname, "_bg_color")), "Use default"));
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
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "hud_panel_healtharmor_progressbar", "Enable status bar"));
	me.TR(me);
		me.TD(me, 1, 2, e = makeNexuizTextLabel(0, "Status bar alignment:"));
			setDependent(e, "hud_panel_healtharmor_progressbar", 1, 1);
	me.TR(me);
		me.TDempty(me, 0.2);
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(2, "hud_panel_healtharmor_baralign", "0", "Left"));
			setDependent(e, "hud_panel_healtharmor_progressbar", 1, 1);
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(2, "hud_panel_healtharmor_baralign", "1", "Right"));
			setDependent(e, "hud_panel_healtharmor_progressbar", 1, 1);
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(2, "hud_panel_healtharmor_baralign", "3", "Inward"));
			setDependent(e, "hud_panel_healtharmor_progressbar", 1, 1);
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(2, "hud_panel_healtharmor_baralign", "4", "Outward"));
			setDependent(e, "hud_panel_healtharmor_progressbar", 1, 1);
	me.TR(me);
		me.TD(me, 1, 2, e = makeNexuizTextLabel(0, "Icon alignment:"));
	me.TR(me);
		me.TDempty(me, 0.2);
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(3, "hud_panel_healtharmor_iconalign", "0", "Left"));
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(3, "hud_panel_healtharmor_iconalign", "1", "Right"));
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(3, "hud_panel_healtharmor_iconalign", "3", "Inward"));
			me.TD(me, 1, 0.95, e = makeNexuizRadioButton(3, "hud_panel_healtharmor_iconalign", "4", "Outward"));
	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "hud_panel_healtharmor_flip", "Flip health and armor positions"));
}
#endif
