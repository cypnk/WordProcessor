
-- Database presets
PRAGMA trusted_schema = OFF;	-- Preemptive defense
PRAGMA cell_size_check = ON;	-- Integrity check
PRAGMA encoding = "UTF-8";	-- Default encoding set to UTF-8
PRAGMA auto_vacuum = "2";	-- File size improvement
PRAGMA temp_store = "2";	-- Memory temp storage for performance
PRAGMA journal_mode = "WAL";	-- Performance improvement

CREATE TABLE documents(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
	excerpt TEXT NOT NULL DEFAULT '',
	created DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);-- --
CREATE INDEX idx_doc_created ON documents ( created );-- --
CREATE INDEX idx_doc_updated ON documents ( updated );-- --

CREATE TRIGGER doc_update AFTER UPDATE ON documents FOR EACH ROW
BEGIN
	UPDATE documents SET updated = CURRENT_TIMESTAMP 
		WHERE id = OLD.id;
END;-- --

CREATE TABLE pages(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
	sort_order INTEGER NOT NULL DEFAULT 0,
	doc_id INTEGER NOT NULL,
	text_dir INTEGER NOT NULL DEFAULT 0,
	excerpt TEXT NOT NULL DEFAULT '',
	
	CONSTRAINT fk_page_doc
		FOREIGN KEY ( doc_id ) 
		REFERENCES documents ( id )
		ON DELETE CASCADE
);-- --
CREATE INDEX idx_page_doc ON pages ( doc_id );-- --
CREATE INDEX idx_page_sort ON pages ( sort_order ASC );-- --

CREATE TABLE text_lines(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
	sort_order INTEGER NOT NULL DEFAULT 0,
	page_id INTEGER NOT NULL,
	cur_pos INTEGER NOT NULL DEFAULT 0,
	text_dir INTEGER NOT NULL DEFAULT 0,
	contents TEXT NOT NULL COLLATE NOCASE,
	
	CONSTRAINT fk_text_page
		FOREIGN KEY ( page_id ) 
		REFERENCES pages ( id )
		ON DELETE CASCADE
);-- --
CREATE INDEX idx_line_page ON text_lines ( page_id );-- --
CREATE INDEX idx_line_sort ON text_lines ( sort_order ASC );-- --

CREATE VIRTUAL TABLE text_search 
	USING fts4( body, tokenize=unicode61 );-- --

CREATE TRIGGER text_insert AFTER INSERT ON text_lines FOR EACH ROW 
WHEN NEW.contents IS NOT ''
BEGIN
	INSERT INTO text_search( docid, body ) 
		VALUES( NEW.id, NEW.contents );
END;-- --

CREATE TRIGGER text_update AFTER UPDATE ON text_lines FOR EACH ROW 
WHEN NEW.contents IS NOT ''
BEGIN
	REPLACE INTO text_search( docid, body )
		VALUES( NEW.id, NEW.contents );
END;-- --

CREATE TRIGGER text_delete BEFORE DELETE ON text_lines FOR EACH ROW 
BEGIN
	DELETE FROM text_search WHERE docid = OLD.id;
END;-- --

CREATE TABLE history(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
	doc_id INTEGER NOT NULL,
	commands TEXT NOT NULL COLLATE NOCASE,
	contents TEXT NOT NULL COLLATE NOCASE,
	
	CONSTRAINT fk_history_doc
		FOREIGN KEY ( doc_id ) 
		REFERENCES documents ( id )
		ON DELETE CASCADE
);-- --
CREATE INDEX idx_history_doc ON history ( doc_id );-- --

CREATE TABLE macros(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
	label TEXT NOT NULL COLLATE NOCASE,
	contents TEXT NOT NULL COLLATE NOCASE
);-- --

CREATE TABLE memos(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
	text_id INTEGER NOT NULL,
	sel_start INTEGER NOT NULL DEFAULT 0,
	sel_end INTEGER NOT NULL DEFAULT 0,
	
	CONSTRAINT fk_text_memo
		FOREIGN KEY ( text_id ) 
		REFERENCES text_lines ( id )
		ON DELETE CASCADE
);-- --
CREATE INDEX idx_memo_text ON memos ( text_id );-- --

CREATE TABLE memo_text(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	memo_id INTEGER NOT NULL,
	contents TEXT NOT NULL COLLATE NOCASE,
	
	CONSTRAINT fk_memo_content
		FOREIGN KEY ( memo_id ) 
		REFERENCES memos ( id )
		ON DELETE CASCADE 
);-- --

CREATE VIRTUAL TABLE memo_search 
	USING fts4( body, tokenize=unicode61 );-- --

CREATE TRIGGER memo_insert AFTER INSERT ON memo_text FOR EACH ROW 
WHEN NEW.contents IS NOT ''
BEGIN
	INSERT INTO memo_search( docid, body ) 
		VALUES( NEW.id, NEW.contents );
END;-- --

CREATE TRIGGER memo_update AFTER UPDATE ON memo_text FOR EACH ROW 
WHEN NEW.contents IS NOT ''
BEGIN
	REPLACE INTO memo_search( docid, body )
		VALUES( NEW.id, NEW.contents );
END;-- --

CREATE TRIGGER memo_delete BEFORE DELETE ON memo_text FOR EACH ROW 
BEGIN
	DELETE FROM memo_search WHERE docid = OLD.id;
END;-- --


CREATE TABLE clipboard(
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	contents TEXT NOT NULL COLLATE NOCASE
);-- --

CREATE VIRTUAL TABLE clip_search 
	USING fts4( body, tokenize=unicode61 );-- --

CREATE TRIGGER clip_insert AFTER INSERT ON clipboard FOR EACH ROW 
WHEN NEW.contents IS NOT ''
BEGIN
	INSERT INTO clip_search( docid, body ) 
		VALUES ( NEW.id, NEW.contents );
END;-- --

CREATE TRIGGER clip_delete BEFORE DELETE ON clipboard FOR EACH ROW 
BEGIN
	DELETE FROM clip_search WHERE docid = OLD.id;
END;-- --

CREATE VIEW content_activity AS SELECT 
	text_lines.id AS text_id,
	text_lines.contents AS text_contents
	FROM text_lines;-- --

CREATE TRIGGER content_maintenance INSTEAD OF 
	UPDATE OF contents ON content_activity
BEGIN 
	UPDATE text_lines SET text_dir = 0 WHERE text_dir < 0;
	UPDATE pages SET text_dir = 0 WHERE text_dir < 0;
	
	DELETE FROM clipboard WHERE contents = '';
	DELETE FROM history WHERE commands = '' AND contents = '';
	DELETE FROM memo_text WHERE contents = '';
	DELETE FROM memos WHERE sel_start = sel_end;
	
	DELETE FROM text_search WHERE docid IN (
		SELECT GROUP_CONCAT( text_lines.id ) AS id 
			FROM text_lines WHERE text_lines.contents = ''
	);
	
	DELETE FROM clip_search WHERE docid IN (
		SELECT GROUP_CONCAT( clipboard.id ) AS id
			FROM clipboard WHERE clipboard.contents = ''
	);
END;-- --


