#ifdef INTERFACE
CLASS(NexuizDemoList) EXTENDS(NexuizListBox)
    METHOD(NexuizDemoList, configureNexuizDemoList, void(entity))
    ATTRIB(NexuizDemoList, rowsPerItem, float, 1)
    METHOD(NexuizDemoList, resizeNotify, void(entity, vector, vector, vector, vector))
    METHOD(NexuizDemoList, drawListBoxItem, void(entity, float, vector, float))
    METHOD(NexuizDemoList, getDemos, void(entity))
    METHOD(NexuizDemoList, startDemo, void(entity))    
    METHOD(NexuizDemoList, demoName, string(entity, float))
    METHOD(NexuizDemoList, clickListBoxItem, void(entity, float, vector))
    METHOD(NexuizDemoList, keyDown, float(entity, float, float, float))
    METHOD(NexuizDemoList, destroy, void(entity))
    METHOD(NexuizDemoList, showNotify, void(entity))
    ATTRIB(NexuizDemoList, listDemo, float, -1)
    ATTRIB(NexuizDemoList, realFontSize, vector, '0 0 0')
    ATTRIB(NexuizDemoList, columnNameOrigin, float, 0)
    ATTRIB(NexuizDemoList, columnNameSize, float, 0)
    ATTRIB(NexuizDemoList, realUpperMargin, float, 0)
    ATTRIB(NexuizDemoList, origin, vector, '0 0 0')
    ATTRIB(NexuizDemoList, itemAbsSize, vector, '0 0 0')
    ATTRIB(NexuizDemoList, lastClickedDemo, float, -1)
    ATTRIB(NexuizDemoList, lastClickedTime, float, 0)
    ATTRIB(NexuizDemoList, filterString, string, string_null)    
ENDCLASS(NexuizDemoList)

entity makeNexuizDemoList();
void StartDemo_Click(entity btn, entity me);
void TimeDemo_Click(entity btn, entity me);
void DemoList_Filter_Change(entity box, entity me);
#endif

#ifdef IMPLEMENTATION

entity makeNexuizDemoList()
{
    entity me;
    me = spawnNexuizDemoList();
    me.configureNexuizDemoList(me);
    return me;
}

void NexuizDemoList_configureNexuizDemoList(entity me)
{
    me.configureNexuizListBox(me);
    me.getDemos(me);    
}

string NexuizDemoList_demoName(entity me, float i )
{
    string s;
    s = search_getfilename(me.listDemo, i);
    s = substring(s, 6, strlen(s) - 6 - 4);  // demos/, .dem
    return s;
}


void NexuizDemoList_getDemos(entity me)
{
    string s;
    
    if(me.filterString)
    	//subdirectory in filterString allowed    
    	s=strcat("demos/*", me.filterString, "*.dem");    	
    else
    	s="demos/*.dem";
	
    //dprint("Search demos with the pattern ", s, "\n");    
	if(me.listDemo >= 0)
		search_end(me.listDemo);
    me.listDemo = search_begin(s, FALSE, TRUE);
    if(me.listDemo < 0)
    	me.nItems=0;
    else
    	me.nItems=search_getsize(me.listDemo);				
}

void NexuizDemoList_destroy(entity me)
{
    search_end(me.listDemo);
}

void NexuizDemoList_resizeNotify(entity me, vector relOrigin, vector relSize, vector absOrigin, vector absSize)
{
    me.itemAbsSize = '0 0 0';
    SUPER(NexuizDemoList).resizeNotify(me, relOrigin, relSize, absOrigin, absSize);

    me.realFontSize_y = me.fontSize / (me.itemAbsSize_y = (absSize_y * me.itemHeight));
    me.realFontSize_x = me.fontSize / (me.itemAbsSize_x = (absSize_x * (1 - me.controlWidth)));
    me.realUpperMargin = 0.5 * (1 - me.realFontSize_y);

    me.columnNameOrigin = me.realFontSize_x;
    me.columnNameSize = 1 - 2 * me.realFontSize_x;
}

void NexuizDemoList_drawListBoxItem(entity me, float i, vector absSize, float isSelected)
{
    string s;
    if(isSelected)
    	draw_Fill('0 0 0', '1 1 0', SKINCOLOR_LISTBOX_SELECTED, SKINALPHA_LISTBOX_SELECTED);
		
    s = me.demoName(me,i);
    s = draw_TextShortenToWidth(s, me.columnNameSize, 0, me.realFontSize);
    draw_Text(me.realUpperMargin * eY + (me.columnNameOrigin + 0.00 * (me.columnNameSize - draw_TextWidth(s, 0, me.realFontSize))) * eX, s, me.realFontSize, '1 1 1', SKINALPHA_TEXT, 0);		
}

void NexuizDemoList_showNotify(entity me)
{
    me.getDemos(me);
}

void DemoList_Filter_Change(entity box, entity me)
{	
    if(me.filterString)
    	strunzone(me.filterString);
    
    if(box.text != "")
    	me.filterString = strzone(box.text);
    else
    	me.filterString = string_null;
		
    me.getDemos(me);
}

void NexuizDemoList_startDemo(entity me)
{
    string s;
    s = me.demoName(me,me.selectedItem);
    localcmd("playdemo demos/", s, ".dem\nwait\ntogglemenu\n");	
}

void StartDemo_Click(entity btn, entity me)
{
    me.startDemo(me);
}

void TimeDemo_Click(entity btn, entity me)
{
    string s;
    s = me.demoName(me,me.selectedItem);
    localcmd("timedemo demos/", s, ".dem\nwait\ntogglemenu\n");	
}

void NexuizDemoList_clickListBoxItem(entity me, float i, vector where)
{
    if(i == me.lastClickedDemo)
        if(time < me.lastClickedTime + 0.3)
        {
            // DOUBLE CLICK!
            me.setSelected(me, i);
            me.startDemo(me);
        }
    me.lastClickedDemo = i;
    me.lastClickedTime = time;
}

float NexuizDemoList_keyDown(entity me, float scan, float ascii, float shift)
{
    if(scan == K_ENTER || scan == K_KP_ENTER) {
        me.startDemo(me);
        return 1;
    }
    else
        return SUPER(NexuizDemoList).keyDown(me, scan, ascii, shift);
}
#endif

