#ifdef INTERFACE
CLASS(NexuizHUDExitDialog) EXTENDS(NexuizRootDialog)
	METHOD(NexuizHUDExitDialog, fill, void(entity))
	ATTRIB(NexuizHUDExitDialog, title, string, "Panel HUD Setup")
	ATTRIB(NexuizHUDExitDialog, color, vector, SKINCOLOR_DIALOG_TEAMSELECT)
	ATTRIB(NexuizHUDExitDialog, intendedWidth, float, 0.4)
	ATTRIB(NexuizHUDExitDialog, rows, float, 18)
	ATTRIB(NexuizHUDExitDialog, columns, float, 4)
	ATTRIB(NexuizHUDExitDialog, name, string, "HUDExit")
ENDCLASS(NexuizHUDExitDialog)

#endif

#ifdef IMPLEMENTATION
void NexuizHUDExitDialog_fill(entity me)
{
	entity e;
	float i;

	me.TR(me);
		me.TD(me, 1, 4, makeNexuizTextLabel(0, "Panel background defaults:"));
	me.TR(me);
		me.TD(me, 1, 1.4, e = makeNexuizTextLabel(0, "Background:"));
			me.TD(me, 1, 1.6, e = makeNexuizTextSlider(strzone(strcat("hud_panel_bg"))));
				e.addValue(e, "Disable", "0");
				e.addValue(e, "border_default", "border_default");
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Color:"));
		me.TD(me, 2, 2.4, e = makeNexuizColorpickerString("hud_panel_bg_color", "hud_panel_bg_color"));
	me.TR(me);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Border size:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_panel_bg_border"));
				e.addValue(e, "Disable", "0");
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i * 2, 0)), strzone(ftos(i * 2)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Alpha:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_panel_bg_alpha"));
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i/10, 1)), strzone(ftos(i/10)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Team color:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_panel_bg_color_team"));
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
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_panel_bg_padding"));
				for(i = 0; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i - 5, 0)), strzone(ftos(i - 5)));
				e.configureNexuizTextSliderValues(e);

	me.TR(me);
		me.TD(me, 1, 1.4, e = makeNexuizTextLabel(0, "HUD Dock:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_dock"));
				e.addValue(e, "Disable", "0");
				e.addValue(e, "Small", "dock_small");
				e.addValue(e, "Medium", "dock_medium");
				e.addValue(e, "Large", "dock_large");
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Color:"));
		me.TD(me, 2, 2.4, e = makeNexuizColorpickerString("hud_dock_color", "hud_dock_color"));
	me.TR(me);
	me.TR(me);	
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Alpha:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_dock_alpha"));
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i/10, 1)), strzone(ftos(i/10)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Team color:"));
			me.TD(me, 1, 2.6, e = makeNexuizTextSlider("hud_dock_color_team"));
				e.addValue(e, "Disable", "0");
				for(i = 1; i <= 10; ++i)
					e.addValue(e, strzone(ftos_decimals(i/10, 1)), strzone(ftos(i/10)));
				e.configureNexuizTextSliderValues(e);
	me.TR(me);
		me.TD(me, 1, 4, makeNexuizTextLabel(0, "Grid settings:"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "hud_configure_grid", "Snap panels to grid"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1.2, e = makeNexuizTextLabel(0, "Grid size:"));
		me.TD(me, 1, 0.2, e = makeNexuizTextLabel(0, "X:"));
			me.TD(me, 1, 1.1, e = makeNexuizTextSlider("hud_configure_grid_xsize"));
				for(i = 1; i <= 14; ++i)
					e.addValue(e, strzone(ftos_decimals(i/200, 3)), strzone(ftos(i/200)));
				e.configureNexuizTextSliderValues(e);
			setDependent(e, "hud_configure_grid", 1, 1);
		me.TD(me, 1, 0.2, e = makeNexuizTextLabel(0, "Y:"));
			me.TD(me, 1, 1.1, e = makeNexuizTextSlider("hud_configure_grid_ysize"));
				for(i = 1; i <= 14; ++i)
					e.addValue(e, strzone(ftos_decimals(i/200, 3)), strzone(ftos(i/200)));
				e.configureNexuizTextSliderValues(e);
			setDependent(e, "hud_configure_grid", 1, 1);

	me.gotoRC(me, me.rows - 1, 0);
		me.TD(me, 1, me.columns, e = makeNexuizCommandButton("Exit setup", '0 0 0', "_hud_configure 0", 1));
}
#endif
