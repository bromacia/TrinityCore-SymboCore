DROP TABLE IF EXISTS `dps_counters`;

CREATE TABLE `dps_counters` (
	`entry` INT UNSIGNED NOT NULL,
	`mode` INT UNSIGNED NOT NULL,
	`guid` INT UNSIGNED NOT NULL,
	`dps` FLOAT NOT NULL,
	`competition` INT UNSIGNED NOT NULL DEFAULT 0,
	PRIMARY KEY (`entry`, `mode`, `guid`, `competition`)
) ENGINE=InnoDB;
