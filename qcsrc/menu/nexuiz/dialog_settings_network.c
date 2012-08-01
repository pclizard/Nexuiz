#ifdef INTERFACE
CLASS(NexuizNetworkSettingsTab) EXTENDS(NexuizTab)
	METHOD(NexuizNetworkSettingsTab, fill, void(entity))
	ATTRIB(NexuizNetworkSettingsTab, title, string, "Network")
	ATTRIB(NexuizNetworkSettingsTab, intendedWidth, float, 0.9)
	ATTRIB(NexuizNetworkSettingsTab, rows, float, 17)
	ATTRIB(NexuizNetworkSettingsTab, columns, float, 6.5)
ENDCLASS(NexuizNetworkSettingsTab)
entity makeNexuizNetworkSettingsTab();
#endif

#ifdef IMPLEMENTATION
entity makeNexuizNetworkSettingsTab()
{
	entity me;
	me = spawnNexuizNetworkSettingsTab();
	me.configureDialog(me);
	return me;
}

void NexuizNetworkSettingsTab_fill(entity me)
{
	entity e;

	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "cl_movement", "Client-side movement prediction"));
	me.TR(me);
		//me.TD(me, 1, 3, e = makeNexuizCheckBox(1, "cl_nolerp", "Network update smoothing"));
	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizCheckBox(0, "shownetgraph", "Show netgraph"));
	me.TR(me);
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Network speed:"));
		me.TD(me, 1, 0.64, e = makeNexuizInputBox(0, "_cl_rate"));
    me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Input packets/s:"));
		me.TD(me, 1, 0.64, e = makeNexuizInputBox(0, "cl_netfps"));
	me.TR(me);
	me.TR(me);
		me.TD(me, 1, 3, e = makeNexuizTextLabel(0, "HTTP downloads:"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 0.8, e = makeNexuizTextLabel(0, "Downloads:"));
		me.TD(me, 1, 2, e = makeNexuizSlider(1, 5, 1, "cl_curl_maxdownloads"));
	me.TR(me);
		me.TDempty(me, 0.2);
		me.TD(me, 1, 0.8, e = makeNexuizTextLabel(0, "Speed (kB/s):"));
		me.TD(me, 1, 0.64, e = makeNexuizInputBox(0, "cl_curl_maxspeed"));
	me.TR(me);
	me.TR(me);
		me.TD(me, 1, 1, e = makeNexuizTextLabel(0, "Client UDP port:"));
		me.TD(me, 1, 0.64, e = makeNexuizInputBox(0, "cl_port"));
}
#endif
