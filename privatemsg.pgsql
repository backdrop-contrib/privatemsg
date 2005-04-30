CREATE TABLE privatemsg (
  id          	SERIAL,
  author      	integer NOT NULL,
  recipient   	integer NOT NULL,
  subject     	varchar(64) NOT NULL,
  message     	text,
  timestamp   	integer NOT NULL,
  newmsg      	smallint NOT NULL,
  hostname    	varchar(255) NOT NULL,
  format        smallint NOT NULL DEFAULT '0',
  folder      	integer NOT NULL DEFAULT '0',
  author_del  	smallint NOT NULL DEFAULT '0',
  recipient_del smallint NOT NULL DEFAULT '0',
  PRIMARY KEY (id)
);

CREATE INDEX privatemsg_folder_index ON folder(privatemsg);

CREATE TABLE privatemsg_folder (
  fid         SERIAL,
  uid         integer NOT NULL,
  name        varchar(255) not null,
  PRIMARY KEY (fid)
);

INSERT INTO privatemsg_folder (uid, name) VALUES (0, 'Sent'); 

CREATE TABLE privatemsg_archive (
  id            SERIAL,
  author        integer NOT NULL,
  recipient     integer NOT NULL,
  subject       VARCHAR(64) NOT NULL,
  message       text NOT NULL,
  timestamp     integer NOT NULL,
  hostname      varchar(255) NOT NULL,
  format        smallint NOT NULL DEFAULT '0',
  folder        integer NOT NULL,
  PRIMARY KEY (id)
);

CREATE INDEX privatemsg_archive_recipient ON privatemsg_archive(recipient);

create or replace function unix_timestamp(timestamp with time zone)
returns int as '
declare
   date alias for $1;
        timezero timestamp;
        offset interval;
begin
        timezero := timestamp ''1970-1-1 00:00'' at time zone ''utc'';
        offset := date-timezero;

   return (extract(''days'' from offset)*86400+
          extract(''hours'' from offset)*3600+
          extract(''minutes'' from offset)*60+
          extract(''seconds'' from offset))::int;
end;
' language 'plpgsql';

create or replace function unix_timestamp(timestamp without time zone)
returns int as '
declare
   date alias for $1;
        timezero timestamp;
        offset interval;
begin
        timezero := timestamp ''1970-1-1 00:00'' at time zone ''utc'';
        offset := date-timezero;

   return (extract(''days'' from offset)*86400+
          extract(''hours'' from offset)*3600+
          extract(''minutes'' from offset)*60+
          extract(''seconds'' from offset))::int;
end;
' language 'plpgsql';
