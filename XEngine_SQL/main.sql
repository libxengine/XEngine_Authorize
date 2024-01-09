/*
 Navicat Premium Data Transfer

 Source Server         : authorize
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 09/01/2024 14:12:10
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Authorize_Announcement
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_Announcement";
CREATE TABLE "Authorize_Announcement" (
  "ID" INTEGER NOT NULL,
  "tszContext" TEXT,
  "tszCreateTime" DATE,
  PRIMARY KEY ("ID")
);

-- ----------------------------
-- Records of Authorize_Announcement
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_BannedAddr
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_BannedAddr";
CREATE TABLE "Authorize_BannedAddr" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "bEnable" blob NOT NULL,
  "tszIPAddr" TEXT NOT NULL,
  "tszLeftTime" DATE,
  "tszCreateTime" DATE NOT NULL
);

-- ----------------------------
-- Records of Authorize_BannedAddr
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_BannedUser
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_BannedUser";
CREATE TABLE "Authorize_BannedUser" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "bEnable" blob NOT NULL,
  "tszUserName" TEXT NOT NULL,
  "tszLeftTime" DATE,
  "tszCreateTime" DATE NOT NULL
);

-- ----------------------------
-- Records of Authorize_BannedUser
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_Serial
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_Serial";
CREATE TABLE "Authorize_Serial" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "UserName" TEXT,
  "SerialNumber" TEXT,
  "MaxTime" TEXT,
  "CardSerialType" integer,
  "bIsUsed" boolean,
  "CreateTime" TEXT NOT NULL
);

-- ----------------------------
-- Records of Authorize_Serial
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_TempVer
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_TempVer";
CREATE TABLE "Authorize_TempVer" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "tszVSerial" TEXT NOT NULL,
  "nVMode" integer NOT NULL,
  "nVTime" integer NOT NULL,
  "nLTime" integer NOT NULL,
  "CreateTime" TEXT NOT NULL
);

-- ----------------------------
-- Records of Authorize_TempVer
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_User
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_User";
CREATE TABLE "Authorize_User" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "UserName" TEXT,
  "Password" TEXT,
  "LeftTime" TEXT,
  "EmailAddr" TEXT,
  "HardCode" TEXT,
  "CardSerialType" integer,
  "PhoneNumber" integer,
  "IDCard" integer,
  "nUserLevel" integer,
  "CreateTime" TEXT NOT NULL
);

-- ----------------------------
-- Records of Authorize_User
-- ----------------------------
INSERT INTO "Authorize_User" VALUES (1, 'admin', '123123aa', '5', '486179@qq.com', 'DAFD2A2DAD', 1, 13699430000, 511025198881118888, 0, '2022-09-21 15:06:45');

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE "sqlite_sequence" (
  "name",
  "seq"
);

-- ----------------------------
-- Records of sqlite_sequence
-- ----------------------------
INSERT INTO "sqlite_sequence" VALUES ('Authorize_BannedUser', 6);
INSERT INTO "sqlite_sequence" VALUES ('Authorize_BannedAddr', 5);
INSERT INTO "sqlite_sequence" VALUES ('Authorize_User', 2);
INSERT INTO "sqlite_sequence" VALUES ('Authorize_TempVer', 1);

-- ----------------------------
-- Auto increment value for Authorize_BannedAddr
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 5 WHERE name = 'Authorize_BannedAddr';

-- ----------------------------
-- Auto increment value for Authorize_BannedUser
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 6 WHERE name = 'Authorize_BannedUser';

-- ----------------------------
-- Auto increment value for Authorize_TempVer
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'Authorize_TempVer';

-- ----------------------------
-- Auto increment value for Authorize_User
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 2 WHERE name = 'Authorize_User';

PRAGMA foreign_keys = true;
