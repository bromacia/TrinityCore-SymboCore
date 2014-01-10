
-- on characters

DROP TABLE IF EXISTS `ninja_inquisitor_logs`;

CREATE TABLE `ninja_inquisitor_logs` (
    `time` INT UNSIGNED NOT NULL,
    `instance` INT UNSIGNED NOT NULL,
    `guid` INT UNSIGNED NOT NULL,
    `event` TEXT NOT NULL
) ENGINE=InnoDB;

DROP TRIGGER IF EXISTS `ninja_inquisitor_delete`;

DELIMITER $$
CREATE TRIGGER `ninja_inquisitor_delete` BEFORE DELETE ON `instance` FOR EACH ROW
BEGIN
    DELETE FROM `ninja_inquisitor_logs` WHERE `instance` = `old`.`id`;
END$$

DELIMITER ;
