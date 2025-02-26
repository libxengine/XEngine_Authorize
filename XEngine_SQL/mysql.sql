/*
 Navicat Premium Dump SQL

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 80041 (8.0.41-0ubuntu0.24.04.1)
 Source Host           : 10.0.2.6:3306
 Source Schema         : XEngine_Authorize

 Target Server Type    : MySQL
 Target Server Version : 80041 (8.0.41-0ubuntu0.24.04.1)
 File Encoding         : 65001

 Date: 26/02/2025 14:14:00
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for Authorize_Announcement
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_Announcement`;
CREATE TABLE `Authorize_Announcement`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `tszContext` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `tszCreateTime` date NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of Authorize_Announcement
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_BannedAddr
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_BannedAddr`;
CREATE TABLE `Authorize_BannedAddr`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `bEnable` tinyint NOT NULL,
  `tszIPAddr` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `tszLeftTime` datetime NULL DEFAULT NULL,
  `tszCreateTime` datetime NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of Authorize_BannedAddr
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_BannedUser
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_BannedUser`;
CREATE TABLE `Authorize_BannedUser`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `bEnable` tinyint(1) NOT NULL,
  `tszUserName` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `tszLeftTime` datetime NULL DEFAULT NULL,
  `tszCreateTime` datetime NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of Authorize_BannedUser
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_Login
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_Login`;
CREATE TABLE `Authorize_Login`  (
  `UserName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL,
  `UserAddr` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL,
  `UserTime` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Authorize_Login
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_Serial
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_Serial`;
CREATE TABLE `Authorize_Serial`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `UserName` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `SerialNumber` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `MaxTime` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL,
  `CardSerialType` int NULL DEFAULT NULL,
  `bIsUsed` tinyint(1) NULL DEFAULT NULL,
  `CreateTime` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of Authorize_Serial
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_TempVer
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_TempVer`;
CREATE TABLE `Authorize_TempVer`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `tszVSerial` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `nVMode` int NOT NULL,
  `nVTime` int NOT NULL,
  `nLTime` int NOT NULL,
  `CreateTime` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of Authorize_TempVer
-- ----------------------------

-- ----------------------------
-- Table structure for Authorize_User
-- ----------------------------
DROP TABLE IF EXISTS `Authorize_User`;
CREATE TABLE `Authorize_User`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `UserName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '用户名',
  `Password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '密码',
  `LeftTime` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '剩余时间',
  `EmailAddr` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '电子邮件',
  `HardCode` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL COMMENT '设备SN',
  `CardSerialType` int NOT NULL,
  `PhoneNumber` bigint NOT NULL COMMENT '电话号码',
  `IDCard` bigint NOT NULL COMMENT '身份证',
  `nUserLevel` int NOT NULL COMMENT '用户等级,数字越小越高',
  `UPTime` datetime NULL DEFAULT NULL COMMENT '登录时间',
  `CreateTime` datetime NOT NULL COMMENT '注册时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of Authorize_User
-- ----------------------------
INSERT INTO `Authorize_User` VALUES (1, 'admin', '123123aa', '0', '486179@qq.com', 'DAFD2A2DAD', 1, 13699430000, 511025198881118888, 0, '2022-09-21 15:06:45', '2022-09-21 15:06:45');

SET FOREIGN_KEY_CHECKS = 1;
