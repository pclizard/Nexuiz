#ifdef INTERFACE
CLASS(XonoticZoomConfigDialog) EXTENDS(XonoticDialog)
	METHOD(XonoticZoomConfigDialog, toString, string(entity))
	METHOD(XonoticZoomConfigDialog, fill, void(entity))
	METHOD(XonoticZoomConfigDialog, showNotify, void(entity))
	ATTRIB(XonoticZoomConfigDialog, title, string, "Zoom Settings")
	ATTRIB(XonoticZoomConfigDialog, color, vector, SKINCOLOR_DIALOG_WEAPONS)
	ATTRIB(XonoticZoomConfigDialog, intendedWidth, float, 0.5)
	ATTRIB(XonoticZoomConfigDialog, rows, float, 10)
	ATTRIB(XonoticZoomConfigDialog, columns, float, 4)
ENDCLASS(XonoticZoomConfigDialog)
#endif

#ifdef IMPLEMENTATION
void XonoticZoomConfigDialog_showNotify(entity me)
{
        loadAllCvars(me);
}

void XonoticZoomConfigDialog_fill(entity me)
{
	entity e;
	
	me.TR(me);
		me.TD(me, 1, 4, makeXonoticTextLabel(0, "Zoom Sensitivity:"));
	me.TR(me);
		me.TD(me, 1, 3, e = makeXonoticSlider(0, 1, 0.1, "cl_zoomsensitivity"));
	me.TR(me);
	me.TR(me);
		me.TD(me, 1, 4, makeXonoticTextLabel(0, "Zoom Speed (set to -1 to disable):"));
	me.TR(me);
		me.TD(me, 1, 3, e = makeXonoticSlider(-1, 16, 0.5, "cl_zoomspeed"));
	me.TR(me);
	me.TR(me);
		me.TD(me, 1, 4, makeXonoticTextLabel(0, "Zoom Factor:"));
	me.TR(me);
		me.TD(me, 1, 3, e = makeXonoticSlider(1, 16, 0.5, "cl_zoomfactor"));
	me.TR(me);
	me.TR(me);
		me.TD(me, 1, me.columns, e = makeXonoticButton("OK", '0 0 0'));
		e.onClick = Dialog_Close;
		e.onClickEntity = me;
}
#endif
