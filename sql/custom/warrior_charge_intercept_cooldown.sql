-- Charge and Intercept trigger the melee cooldown
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_reset_melee';
INSERT INTO `spell_script_names` VALUES	(-100, 'spell_warr_reset_melee'), (20252, 'spell_warr_reset_melee');
