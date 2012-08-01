#ifdef INTERFACE
CLASS(NexuizPlayerSettingsTab) EXTENDS(NexuizTab)
	METHOD(NexuizPlayerSettingsTab, fill, void(entity))
	METHOD(NexuizPlayerSettingsTab, draw, void(entity))
	ATTRIB(NexuizPlayerSettingsTab, title, string, "Player Setup")
	ATTRIB(NexuizPlayerSettingsTab, intendedWidth, float, 0.9)
	ATTRIB(NexuizPlayerSettingsTab, rows, float, 22)
	ATTRIB(NexuizPlayerSettingsTab, columns, float, 6.5)
	ATTRIB(NexuizPlayerSettingsTab, playerNameLabel, entity, NULL)
	ATTRIB(NexuizPlayerSettingsTab, playerNameLabelAlpha, float, 0)
ENDCLASS(NexuizPlayerSettingsTab)
entity makeNexuizPlayerSettingsTab();

void HUDSetup_Join_Click(entity me, entity btn);
#endif

#ifdef IMPLEMENTATION

entity makeNexuizPlayerSettingsTab()
{
	entity me;
	me = spawnNexuizPlayerSettingsTab();
	me.configureDialog(me);
	return me;
}
void NexuizPlayerSettingsTab_draw(entity me)
{
	if(cvar_string("_cl_name") == "Player")
		me.playerNameLabel.alpha = ((mod(time * 2, 2) < 1) ? 1 : 0);
	else
		me.playerNameLabel.alpha = me.playerNameLabelAlpha;
	SUPER(NexuizPlayerSettingsTab).draw(me);
}
void NexuizPlayerSettingsTab_fill(entity me)
{
	entity e, pms, sl, label, e0, box;
	float i, r, m, n;

	me.TR(me);
		me.TD(me, 1, 0.5, me.playerNameLabel = makeNexuizTextLabel(0, "Name:"));
			me.playerNameLabelAlpha = me.playerNameLabel.alpha;
		me.TD(me, 1, 2.5, label = makeNexuizTextLabel(0, string_null));
			label.allowCut = 1;
			label.allowColors = 1;
			label.alpha = 1;
	me.TR(me);
		me.TD(me, 1, 3.0, box = makeNexuizInputBox(1, "_cl_name"));
			box.forbiddenCharacters = "\r\n\\\"$"; // don't care, isn't getting saved
			box.maxLength = -63; // negativ means encoded length in bytes
			label.textEntity = box;
	me.TR(me);
		me.TD(me, 5, 1, e = makeNexuizColorpicker(box));
		me.TD(me, 5, 2, e = makeNexuizCharmap(box));
	me.TR(me);
	me.TR(me);
	me.TR(me);
	me.TR(me);
	me.TR(me);
	me.TR(me);
	me.gotoRC(me, 8, 0.0);
		pms = makeNexuizPlayerModelSelector();
		me.TD(me, 1, 0.6, e = makeNexuizTextLabel(1, "Model:"));
		me.TD(me, 1, 0.3, e = makeNexuizButton("<<", '0 0 0'));
			e.onClick = PlayerModelSelector_Prev_Click;
			e.onClickEntity = pms;
		me.TD(me, me.rows - me.currentRow - 1, 1.8, pms);
		me.TD(me, 1, 0.3, e = makeNexuizButton(">>", '0 0 0'));
			e.onClick = PlayerModelSelector_Next_Click;
			e.onClickEntity = pms;
	me.TR(me);
		r = me.currentRow;
		m = me.rows - r - 2;
		n = 16 - !cvar("developer");
		m = m / (n - 1);
		for(i = 0; i < n; ++i)
		{
			me.gotoRC(me, r + i * m, 0.1);
			me.TDNoMargin(me, m, 0.2, e = makeNexuizColorButton(1, 0, i), '0 1 0');
		}
		for(i = 0; i < n; ++i)
		{
			me.gotoRC(me, r + i * m, 0.4);
			me.TDNoMargin(me, m, 0.2, e = makeNexuizColorButton(2, 1, i), '0 1 0');
		}

	me.gotoRC(me, 0, 3.5); me.setFirstColumn(me, me.currentColumn);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Field of view:"));
		me.TD(me, 1, 2, e = makeNexuizSlider(60, 130, 1, "fov"));
	me.TR(me);
		sl = makeNexuizSlider(0.45, 0.75, 0.01, "cl_bobcycle");
		me.TD(me, 1, 1, e = makeNexuizSliderCheckBox(0, 1, sl, "View bobbing:"));
		makeMulti(sl, "cl_bob2cycle");
		me.TD(me, 1, 2, sl);
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Zoom factor:"));
		me.TD(me, 1, 2, e = makeNexuizSlider(2, 16, 0.5, "cl_zoomfactor"));
	me.TR(me);
		sl = makeNexuizSlider(1, 8, 0.5, "cl_zoomspeed");
		me.TD(me, 1, 1, e = makeNexuizSliderCheckBox(-1, 1, sl, "Zoom speed:"));
		me.TD(me, 1, 2, sl);
	me.TR(me);
		me.TD(me, 1, 1.5, e = makeNexuizButton("Weapon settings...", '0 0 0'));
			e.onClick = DialogOpenButton_Click;
			e.onClickEntity = main.weaponsDialog;
		me.TD(me, 1, 1.5, e0 = makeNexuizTextLabel(0, string_null));
			e0.textEntity = main.weaponsDialog;
			e0.allowCut = 1;
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Crosshair:"));
		me.TD(me, 1, 2, e = makeNexuizCheckBox(0, "crosshair_per_weapon", "Per weapon"));
	me.TR(me);
		me.TDempty(me, 0.2);
		for(i = 1; i <= 10; ++i) {
			me.TDNoMargin(me, 1, 2 / 10, e = makeNexuizCrosshairButton(3, i), '1 1 0');
			setDependent(e, "crosshair_per_weapon", 0, 0);
		}
		// show a larger preview of the selected crosshair
		me.TDempty(me, 0.2);
		me.TDNoMargin(me, 2, 2 / 5, e = makeNexuizCrosshairButton(4, -1), '1 1 0'); // crosshair -1 makes this a preview
		setDependent(e, "crosshair_per_weapon", 0, 0);
	me.TR(me);
		me.TDempty(me, 0.2);
		for(i = 11; i <= 20; ++i) {
			me.TDNoMargin(me, 1, 2 / 10, e = makeNexuizCrosshairButton(3, i), '1 1 0');
			setDependent(e, "crosshair_per_weapon", 0, 0);
		}
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Crosshair size:"));
		me.TD(me, 1, 1.8, e = makeNexuizSlider(0.10, 1.5, 0.05, "crosshair_size"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Crosshair alpha:"));
		me.TD(me, 1, 1.8, e = makeNexuizSlider(0, 1, 0.1, "crosshair_alpha"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Crosshair color:"));
		me.TD(me, 2, 1.8, e = makeNexuizColorpickerString("crosshair_color", "crosshair_color"));
		setDependent(e, "crosshair_color_per_weapon", 0, 0);
	me.TR(me);
		me.TDempty(me, 0.3);
		me.TD(me, 1, 0.7, e = makeNexuizCheckBox(0, "crosshair_color_per_weapon", "Per weapon"));
	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "crosshair_dot", "Enable centered dot"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 0.5, e = makeNexuizTextLabel(0, "Size:"));
		me.TD(me, 1, 0.9, e = makeNexuizSlider(0.2, 2, 0.1, "crosshair_dot_size"));
			setDependent(e, "crosshair_dot", 1, 1);
		me.TD(me, 1, 0.5, e = makeNexuizTextLabel(0, "Alpha:"));
		me.TD(me, 1, 0.9, e = makeNexuizSlider(0.10, 1, 0.1, "crosshair_dot_alpha"));
			setDependent(e, "crosshair_dot", 1, 1);
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Hit test:"));
		me.TD(me, 1, 2/3, e = makeNexuizRadioButton(5, "crosshair_hittest", "0",    "None"));
		me.TD(me, 1, 2/3, e = makeNexuizRadioButton(5, "crosshair_hittest", "1",    "TrueAim"));
		me.TD(me, 1, 2/3, e = makeNexuizRadioButton(5, "crosshair_hittest", "1.25", "Enemies"));
	me.TR(me);
		me.TDempty(me, 0.4);
		me.TD(me, 1, 2.2, e = makeNexuizButton("Waypoints setup...", '0 0 0'));
			e.onClick = DialogOpenButton_Click;
			e.onClickEntity = main.waypointDialog;
		me.TDempty(me, 0.5);
	me.TR(me);
		me.TDempty(me, 0.4);
		me.TD(me, 1, 2.2, e = makeNexuizButton("Enter HUD editor", '0 0 0'));
			e.onClick = HUDSetup_Join_Click;
			e.onClickEntity = me;
		me.TDempty(me, 0.5);
	me.TR(me);
	me.TR(me);
	#ifdef ALLOW_FORCEMODELS
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Force models:"));
		me.TD(me, 1, 2/3, e = makeNexuizRadioButton(6, string_null, string_null, "None"));
		me.TD(me, 1, 2/3, e = makeNexuizRadioButton(6, "cl_forceplayermodelsfromnexuiz", string_null, "Custom"));
		me.TD(me, 1, 2/3, e = makeNexuizRadioButton(6, "cl_forceplayermodels", string_null, "All"));
	#endif
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "cl_gentle", "Disable gore effects"));
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Gibs:"));
		me.TD(me, 1, 2, e = makeNexuizTextSlider("cl_nogibs"));
			e.addValue(e, "None", "1");
			e.addValue(e, "Few", "0.75");
			e.addValue(e, "Many", "0.5");
			e.addValue(e, "Lots", "0");
			e.configureNexuizTextSliderValues(e);
			setDependent(e, "cl_gentle", 0, 0);
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Damage splash:"));
		me.TD(me, 1, 2, e = makeNexuizSlider(0, 1, 0.05, "hud_damage"));

	me.gotoRC(me, me.rows - 1, 0);
		me.TD(me, 1, me.columns, makeNexuizCommandButton("Apply immediately", '0 0 0', "color -1 -1;name \"$_cl_name\";cl_cmd sendcvar cl_weaponpriority;sendcvar cl_zoomfactor;sendcvar cl_zoomspeed;sendcvar cl_autoswitch;sendcvar cl_shownames;sendcvar cl_forceplayermodelsfromnexuiz;sendcvar cl_forceplayermodels;playermodel $_cl_playermodel;playerskin $_cl_playerskin", COMMANDBUTTON_APPLY));
}
void HUDSetup_Join_Click(entity me, entity btn)
{
	if(! (gamestatus & (GAME_CONNECTED | GAME_ISSERVER)))
	{
		localcmd("map hudsetup/hudsetup", "\n");
	}
	else
		localcmd("togglemenu 0\n");
	localcmd("_hud_configure 1", "\n");
}
#endif
