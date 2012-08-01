#ifdef INTERFACE
CLASS(NexuizServerInfoDialog) EXTENDS(NexuizDialog)
	METHOD(NexuizServerInfoDialog, fill, void(entity))
	METHOD(NexuizServerInfoDialog, loadServerInfo, void(entity, float))
	ATTRIB(NexuizServerInfoDialog, title, string, "Server Information")
	ATTRIB(NexuizServerInfoDialog, color, vector, SKINCOLOR_DIALOG_SERVERINFO)
	ATTRIB(NexuizServerInfoDialog, intendedWidth, float, 0.68)
	ATTRIB(NexuizServerInfoDialog, rows, float, 15)
	ATTRIB(NexuizServerInfoDialog, columns, float, 12)

	ATTRIB(NexuizServerInfoDialog, currentServerName, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerCName, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerType, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerMap, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerPlayers, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerNumPlayers, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerNumBots, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerMod, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerVersion, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerPing, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerKey, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerID, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerEncrypt, string, string_null)
	ATTRIB(NexuizServerInfoDialog, currentServerPure, string, string_null)

	ATTRIB(NexuizServerInfoDialog, nameLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, cnameLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, typeLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, mapLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, rawPlayerList, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, numPlayersLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, numBotsLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, modLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, versionLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, pingLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, keyLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, idLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, encryptLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, canConnectLabel, entity, NULL)
	ATTRIB(NexuizServerInfoDialog, pureLabel, entity, NULL)
ENDCLASS(NexuizServerInfoDialog)

float SLIST_FIELD_NAME;
float SLIST_FIELD_CNAME;
float SLIST_FIELD_QCSTATUS;
float SLIST_FIELD_MAP;
float SLIST_FIELD_PLAYERS;
float SLIST_FIELD_NUMHUMANS;
float SLIST_FIELD_MAXPLAYERS;
float SLIST_FIELD_NUMBOTS;
float SLIST_FIELD_MOD;
float SLIST_FIELD_PING;
void Join_Click(entity btn, entity me);
#endif

#ifdef IMPLEMENTATION
void NexuizServerInfoDialog_loadServerInfo(entity me, float i)
{
	float m, pure, j;
	string s, typestr, versionstr, numh, maxp, k, v;

	if(me.currentServerName)
		strunzone(me.currentServerName);
	me.currentServerName = string_null;
	if(me.currentServerCName)
		strunzone(me.currentServerCName);
	me.currentServerCName = string_null;
	if(me.currentServerType)
		strunzone(me.currentServerType);
	me.currentServerType = string_null;
	if(me.currentServerMap)
		strunzone(me.currentServerMap);
	me.currentServerMap = string_null;
	if(me.currentServerPlayers)
		strunzone(me.currentServerPlayers);
	me.currentServerPlayers = string_null;
	if(me.currentServerNumPlayers)
		strunzone(me.currentServerNumPlayers);
	me.currentServerNumPlayers = string_null;
	if(me.currentServerNumBots)
		strunzone(me.currentServerNumBots);
	me.currentServerNumBots = string_null;
	if(me.currentServerMod)
		strunzone(me.currentServerMod);
	me.currentServerMod = string_null;
	if(me.currentServerVersion)
		strunzone(me.currentServerVersion);
	me.currentServerVersion = string_null;
	if(me.currentServerPing)
		strunzone(me.currentServerPing);
	me.currentServerPing = string_null;
	if(me.currentServerKey)
		strunzone(me.currentServerKey);
	me.currentServerKey = string_null;
	if(me.currentServerID)
		strunzone(me.currentServerID);
	me.currentServerID = string_null;
	// not zoned!
	//if(me.currentServerEncrypt)
	//	strunzone(me.currentServerEncrypt);
	//me.currentServerEncrypt = string_null;
	if(me.currentServerPure)
		strunzone(me.currentServerPure);
	me.currentServerPure = string_null;

	SLIST_FIELD_NAME = gethostcacheindexforkey("name");
	me.currentServerName = strzone(gethostcachestring(SLIST_FIELD_NAME, i));
	me.nameLabel.setText(me.nameLabel, me.currentServerName);

	SLIST_FIELD_CNAME = gethostcacheindexforkey("cname");
	me.currentServerCName = strzone(gethostcachestring(SLIST_FIELD_CNAME, i));
	me.cnameLabel.setText(me.cnameLabel, me.currentServerCName);

	pure = -1;
	typestr = "N/A";
	versionstr = "N/A";

	SLIST_FIELD_QCSTATUS = gethostcacheindexforkey("qcstatus");
	s = gethostcachestring(SLIST_FIELD_QCSTATUS, i);
	m = tokenizebyseparator(s, ":");
	if(m >= 2)
	{
		typestr = argv(0);
		versionstr = argv(1);
	}
	for(j = 2; j < m; ++j)
	{
		if(argv(j) == "")
			break;
		k = substring(argv(j), 0, 1);
		v = substring(argv(j), 1, -1);
		if(k == "P")
			pure = stof(v);
	}

	me.currentServerType = strzone(typestr);
	me.typeLabel.setText(me.typeLabel, me.currentServerType);

	SLIST_FIELD_MAP = gethostcacheindexforkey("map");
	me.currentServerMap = strzone(gethostcachestring(SLIST_FIELD_MAP, i));
	me.mapLabel.setText(me.mapLabel, me.currentServerMap);

	SLIST_FIELD_PLAYERS = gethostcacheindexforkey("players");
	me.currentServerPlayers = strzone(gethostcachestring(SLIST_FIELD_PLAYERS, i));
	me.rawPlayerList.setPlayerList(me.rawPlayerList, me.currentServerPlayers);

	SLIST_FIELD_NUMHUMANS = gethostcacheindexforkey("numhumans");
	numh = ftos(gethostcachenumber(SLIST_FIELD_NUMHUMANS, i));
	SLIST_FIELD_MAXPLAYERS = gethostcacheindexforkey("maxplayers");
	maxp = ftos(gethostcachenumber(SLIST_FIELD_MAXPLAYERS, i));
	me.currentServerNumPlayers = strzone(strcat(numh,"/",maxp));
	me.numPlayersLabel.setText(me.numPlayersLabel, me.currentServerNumPlayers);

	SLIST_FIELD_NUMBOTS = gethostcacheindexforkey("numbots");
	s = ftos(gethostcachenumber(SLIST_FIELD_NUMBOTS, i));
	me.currentServerNumBots = strzone(s);
	me.numBotsLabel.setText(me.numBotsLabel, me.currentServerNumBots);

	SLIST_FIELD_MOD = gethostcacheindexforkey("mod");
	me.currentServerMod = strzone(gethostcachestring(SLIST_FIELD_MOD, i));
	me.modLabel.setText(me.modLabel, me.currentServerMod);

	me.currentServerVersion = strzone(versionstr);
	me.versionLabel.setText(me.versionLabel, me.currentServerVersion);

	me.currentServerPure = ((pure < 0) ? "N/A" : (pure == 0) ? "Official settings" : sprintf("%d modified settings", pure));
	me.currentServerPure = strzone(me.currentServerPure);
	me.pureLabel.setText(me.pureLabel, me.currentServerPure);

	SLIST_FIELD_PING = gethostcacheindexforkey("ping");
	s = ftos(gethostcachenumber(SLIST_FIELD_PING, i));
	me.currentServerPing = strzone(s);
	me.pingLabel.setText(me.pingLabel, me.currentServerPing);

	s = crypto_getidfp(me.currentServerCName);
	if not(s)
		s = "N/A";
	me.currentServerID = strzone(s);
	me.idLabel.setText(me.idLabel, me.currentServerID);

	s = crypto_getkeyfp(me.currentServerCName);
	if not(s)
		s = "N/A";
	me.currentServerKey = strzone(s);
	me.keyLabel.setText(me.keyLabel, me.currentServerKey);

	s = crypto_getencryptlevel(me.currentServerCName);
	if(s == "")
	{
		if(cvar("crypto_aeslevel") >= 3)
			me.currentServerEncrypt = "N/A (can't connect)";
		else
			me.currentServerEncrypt = "N/A";
	}
	else switch(stof(substring(s, 0, 1)))
	{
		case 0:
			if(cvar("crypto_aeslevel") >= 3)
				me.currentServerEncrypt = "not supported (can't connect)";
			else
				me.currentServerEncrypt = "not supported (won't encrypt)";
			break;
		case 1:
			if(cvar("crypto_aeslevel") >= 2)
				me.currentServerEncrypt = "supported (will encrypt)";
			else
				me.currentServerEncrypt = "supported (won't encrypt)";
			break;
		case 2:
			if(cvar("crypto_aeslevel") >= 1)
				me.currentServerEncrypt = "requested (will encrypt)";
			else
				me.currentServerEncrypt = "requested (won't encrypt)";
			break;
		case 3:
			if(cvar("crypto_aeslevel") <= 0)
				me.currentServerEncrypt = "required (can't connect)";
			else
				me.currentServerEncrypt = "required (will encrypt)";
			break;
	}
	me.encryptLabel.setText(me.encryptLabel, me.currentServerEncrypt);
}

void NexuizServerInfoDialog_fill(entity me)
{
	entity e;
	me.TR(me);
		me.TD(me, 1, me.columns, e = makeNexuizTextLabel(0.5, ""));
			e.colorL = SKINCOLOR_SERVERINFO_NAME;
			e.allowCut = 1;
			me.nameLabel = e;
	me.TR(me);
		me.TD(me, 1, me.columns, e = makeNexuizTextLabel(0.5, ""));
			e.colorL = SKINCOLOR_SERVERINFO_IP;
			e.allowCut = 1;
			me.cnameLabel = e;

	me.TR(me);
		me.TD(me, 1, 5.5, e = makeNexuizTextLabel(0, "Players:"));
	me.TR(me);
		me.TD(me, me.rows - 4, 6, e = makeNexuizPlayerList());
			me.rawPlayerList = e;

	me.gotoRC(me, 1, 6.25); me.setFirstColumn(me, me.currentColumn);

	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Type:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.typeLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Map:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.mapLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Gameplay:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.pureLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Players:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.numPlayersLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Bots:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.numBotsLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Mod:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.modLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Version:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.versionLabel = e;
	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Ping:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.pingLabel = e;

	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "CA:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.keyLabel = e;

	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Key:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.idLabel = e;

	me.TR(me);
		me.TD(me, 1, 1.75, e = makeNexuizTextLabel(0, "Encryption:"));
		me.TD(me, 1, 4.0, e = makeNexuizTextLabel(0, ""));
			e.allowCut = 1;
			me.encryptLabel = e;

	me.gotoRC(me, me.rows - 1, 0);

		me.TD(me, 1, me.columns - 6, e = makeNexuizButton("Close", '0 0 0'));
			e.onClick = Dialog_Close;
			e.onClickEntity = me;
		me.TD(me, 1, me.columns - 6, e = makeNexuizButton("Join!", '0 0 0'));
			e.onClick = Join_Click;
			e.onClickEntity = me;
}

void Join_Click(entity btn, entity me)
{
	localcmd("connect ", me.currentServerCName, "\n");
}

#endif
