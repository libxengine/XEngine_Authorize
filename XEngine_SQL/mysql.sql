/*
 Navicat Premium Data Transfer

 Source Server         : 本地mysql
 Source Server Type    : MySQL
 Source Server Version : 50726
 Source Host           : localhost:3306
 Source Schema         : xengine_authorize

 Target Server Type    : MySQL
 Target Server Version : 50726
 File Encoding         : 65001

 Date: 04/07/2024 20:51:48
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for Authorize_Announcement
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_Announcement`;
CREATE TABLE `Authorize_Announcement`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `tszContext` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `tszCreateTime` date NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 0 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_Announcement
-- ----------------------------


-- ----------------------------
-- Table structure for Authorize_BannedAddr
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_BannedAddr`;
CREATE TABLE `Authorize_BannedAddr`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `bEnable` tinyint(4) NOT NULL,
  `tszIPAddr` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `tszLeftTime` date NULL DEFAULT NULL,
  `tszCreateTime` date NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 0 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_BannedAddr
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_BannedUser
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_BannedUser`;
CREATE TABLE `Authorize_BannedUser`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `bEnable` tinyint(1) NOT NULL,
  `tszUserName` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `tszLeftTime` date NULL DEFAULT NULL,
  `tszCreateTime` date NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 0 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_BannedUser
-- ----------------------------


-- ----------------------------
-- Table structure for Authorize_Serial
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_Serial`;
CREATE TABLE `Authorize_Serial`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `UserName` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `SerialNumber` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `MaxTime` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `CardSerialType` int(11) NULL DEFAULT NULL,
  `bIsUsed` tinyint(1) NULL DEFAULT NULL,
  `CreateTime` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 0 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_Serial
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_TempVer
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_TempVer`;
CREATE TABLE `Authorize_TempVer`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `tszVSerial` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `nVMode` int(11) NOT NULL,
  `nVTime` int(11) NOT NULL,
  `nLTime` int(11) NOT NULL,
  `CreateTime` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 0 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_TempVer
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_User
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_User`;
CREATE TABLE `Authorize_User`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `UserName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '用户名',
  `Password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '密码',
  `LeftTime` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '剩余时间',
  `EmailAddr` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '电子邮件',
  `HardCode` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL COMMENT '设备SN',
  `CardSerialType` int(11) NOT NULL,
  `PhoneNumber` bigint(20) NOT NULL COMMENT '电话号码',
  `IDCard` bigint(20) NOT NULL COMMENT '身份证',
  `nUserLevel` int(11) NOT NULL COMMENT '用户等级,数字越小越高',
  `UPTime` datetime NULL DEFAULT NULL COMMENT '登录时间',
  `CreateTime` datetime NOT NULL COMMENT '注册时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 0 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_User
-- ----------------------------
INSERT INTO `Authorize_User` VALUES (1, 'admin', '123123aa', '0', '486179@qq.com', 'DAFD2A2DAD', 1, 13699430000, 511025198881118888, 0, '2022-09-21 15:06:45', '2022-09-21 15:06:45');
-- ----------------------------
-- Table structure for Sqlite_Sequence
-- ----------------------------
DROP TABLE IF EXISTS `Sqlite_Sequence`;
CREATE TABLE `Sqlite_Sequence`  (
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL,
  `seq` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Sqlite_Sequence
-- ----------------------------
INSERT INTO `Sqlite_Sequence` VALUES ('Authorize_BannedUser', 6);
INSERT INTO `Sqlite_Sequence` VALUES ('Authorize_BannedAddr', 5);
INSERT INTO `Sqlite_Sequence` VALUES ('Authorize_TempVer', 1);
INSERT INTO `Sqlite_Sequence` VALUES ('Authorize_Serial', 10);
INSERT INTO `Sqlite_Sequence` VALUES ('Authorize_User', 5);

SET FOREIGN_KEY_CHECKS = 1;
