/*
 Navicat Premium Data Transfer

 Source Server         : authorize
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 29/12/2022 13:14:11
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Authorize_BannedAddr
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_BannedAddr";
CREATE TABLE "Authorize_BannedAddr" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "tszIPStart" TEXT,
  "tszIPEnd" TEXT,
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
  "tszUserName" TEXT,
  "tszCreateTime" DATE NOT NULL
);

-- ----------------------------
-- Records of Authorize_BannedUser
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_NetVer
-- ----------------------------
DROP TABLE IF EXISTS "Authorize_NetVer";
CREATE TABLE "Authorize_NetVer" (
  "ID" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "VerSerial" TEXT NOT NULL,
  "VerMode" integer NOT NULL,
  "TryTime" integer NOT NULL,
  "CreateTime" TEXT NOT NULL
);

-- ----------------------------
-- Records of Authorize_NetVer
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
INSERT INTO "sqlite_sequence" VALUES ('Authorize_User', 1);

-- ----------------------------
-- Auto increment value for Authorize_User
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'Authorize_User';

PRAGMA foreign_keys = true;
