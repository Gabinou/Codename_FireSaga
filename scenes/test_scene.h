
// DEFAULT LINE LAST with PRIORITY CONDITIONS FIRST

// unit id, pc_alive, pc_recruited, npc_alive, misc, chapter_min, line string
// REGISTER_SCENE_START(CURRENT_FILENAME")
REGISTER_LINE(UNIT_NAME_ERWIN,0,0,0,0,0, "Erwin First Line: Default") 
REGISTER_LINE(UNIT_NAME_KIARA, UNIT_PC_ALIVE_KIARA + UNIT_PC_ALIVE_ERWIN,0,0,0,0, "Kiara First Line: First conditional of a group of lines") 
REGISTER_LINE(UNIT_NAME_KIARA, UNIT_PC_ALIVE_KIARA,0,0,0,0, "Kiara First Line: Second conditional of a group of lines") 
REGISTER_LINE(UNIT_NAME_KIARA, UNIT_PC_ALIVE_ERWIN,0,0,0,0, "Kiara First Line: Third conditional of a group of lines") 
REGISTER_LINE(UNIT_NAME_KIARA, 0,0,0,0,0, "Kiara First Line: Default line") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0, UNIT_PC_RECRUITED_KIARA,0,0,0, "Erwin Second Line: First Conditional") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0, UNIT_PC_RECRUITED_KIARA + UNIT_PC_RECRUITED_ERWIN,0,0,0, 
"Erwin Second Line: Second Conditional") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0, UNIT_PC_RECRUITED_ERWIN,0,0,0, 
"Erwin Second Line: Third Conditional") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0,0,0,0,0,
"Erwin Second Line: Default") 
REGISTER_LINE(UNIT_NAME_KIARA, 0,0, UNIT_NPC_ALIVE_ZINEDAN,0,0, 
"Kiara Second Line: First conditional") 
REGISTER_LINE(UNIT_NAME_KIARA, 0,0, UNIT_NPC_ALIVE_ZIDINE,0,0, 
"Kiara Second Line: Second conditional") 
REGISTER_LINE(UNIT_NAME_KIARA, 0,0, UNIT_NPC_ALIVE_ZIDINE + UNIT_NPC_ALIVE_ZINEDAN,0,0, 
"Kiara Second Line: Third conditional") 
REGISTER_LINE(UNIT_NAME_KIARA, 0,0,0,0,0,
"Kiara Second Line: Default") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0,0,0,0,1, 
"Erwin Third Line: First Conditional") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0,0,0,0,2, 
"Erwin Third Line: Second Conditional") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0,0,0,0,3, 
"Erwin Third Line: Third Conditional") 
REGISTER_LINE(UNIT_NAME_ERWIN, 0,0,0,0,0,
"Erwin Third Line: Default")
REGISTER_LINE(UNIT_NAME_KIARA, UNIT_PC_ALIVE_KIARA + UNIT_PC_ALIVE_ERWIN,UNIT_PC_RECRUITED_KIARA + UNIT_PC_RECRUITED_ERWIN, UNIT_NPC_ALIVE_ZIDINE + UNIT_NPC_ALIVE_ZINEDAN,0,3, 
"Kiara Third Line: First Conditional") 
REGISTER_LINE(UNIT_NAME_KIARA, 0,0,0,0,0,"Kiara Third Line: Default") 
// REGISTER_SCENE_END(CURRENT_FILENAME")
