DELETE FROM `disables` WHERE `sourceType` = 0 AND `entry` IN (69675, 69712, 69845, 70117, 70127, 71053, 71054, 71055, 72528, 72529, 72530);

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_sindragosa_collision_filter';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
    (69845, 'spell_sindragosa_collision_filter'), (70117, 'spell_sindragosa_collision_filter'), (70127, 'spell_sindragosa_collision_filter'),
    (71053, 'spell_sindragosa_collision_filter'), (71054, 'spell_sindragosa_collision_filter'), (71055, 'spell_sindragosa_collision_filter'),
    (72528, 'spell_sindragosa_collision_filter'), (72529, 'spell_sindragosa_collision_filter'), (72530, 'spell_sindragosa_collision_filter');
