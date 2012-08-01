#ifdef INTERFACE
CLASS(NexuizWaypointDialog) EXTENDS(NexuizDialog)
	METHOD(NexuizWaypointDialog, toString, string(entity))
	METHOD(NexuizWaypointDialog, fill, void(entity))
	METHOD(NexuizWaypointDialog, showNotify, void(entity))
	ATTRIB(NexuizWaypointDialog, title, string, "Waypoints")
	ATTRIB(NexuizWaypointDialog, color, vector, SKINCOLOR_DIALOG_RADAR)
	ATTRIB(NexuizWaypointDialog, intendedWidth, float, 0.7)
	ATTRIB(NexuizWaypointDialog, rows, float, 6)
	ATTRIB(NexuizWaypointDialog, columns, float, 4)
ENDCLASS(NexuizWaypointDialog)
#endif

#ifdef IMPLEMENTATION
void NexuizWaypointDialog_showNotify(entity me)
{
        loadAllCvars(me);
}
string NexuizWaypointDialog_toString(entity me)
{
	return "XXX";
}
void NexuizWaypointDialog_fill(entity me)
{
	entity e;
	
	me.TR(me);
		me.TD(me, 1, 4, makeNexuizTextLabel(0, "Waypoint settings:"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 3.8, e = makeNexuizCheckBox(1, "cl_hidewaypoints", "Show base waypoints"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 0.8, e = makeNexuizTextLabel(0, "Waypoint scale:"));
			me.TD(me, 1, 3, e = makeNexuizSlider(0.5, 1.5, 0.01, "g_waypointsprite_scale"));
				setDependent(e, "cl_hidewaypoints", 0, 0);
	me.TR(me);
 		me.TDempty(me, 0.2);
		me.TD(me, 1, 0.8, e = makeNexuizTextLabel(0, "Waypoint alpha:"));
			me.TD(me, 1, 3, e = makeNexuizSlider(0.1, 1, 0.01, "g_waypointsprite_alpha"));
				setDependent(e, "cl_hidewaypoints", 0, 0);
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 0.8, e = makeNexuizTextLabel(0, "Show names:"));
		me.TD(me, 1, 3, e = makeNexuizTextSlider("cl_shownames"));
			e.addValue(e, "Never", "0");
			e.addValue(e, "Teammates", "1");
			e.addValue(e, "All players", "2");
			e.configureNexuizTextSliderValues(e);
	me.gotoRC(me, me.rows - 1, 0);
		me.TD(me, 1, me.columns, e = makeNexuizButton("OK", '0 0 0'));
			e.onClick = Dialog_Close;
			e.onClickEntity = me;
}

#endif
