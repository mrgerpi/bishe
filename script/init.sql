Drop Database If Exists thriftMock;
Create Database thriftMock;
Use thriftMock;

Create Table `User`(
	`userId` int not null auto_increment,
	`userName` varchar(30) not null,
	`password` varchar(30) not null,
	Primary Key(`userId`)
) Engine=InnoDB Default Charset=utf8;

Insert Into `User` (userName, password) Values ("451733087@qq.com", "test");

Create Table `ServiceMetaData` (
	`serviceId` int not null auto_increment,
	`serviceName` varchar(30) not null,
	`idl` varchar(30) not null,
	Primary Key(`serviceId`)
) Engine=InnoDB Default Charset=utf8;

Create Table `ServicePatterm`(
	`servicePattermId` int not null auto_increment,
	`userId` int not null,
	`clientServcieId` int not null,
	`serverServiceIds` varchar(50) not null,
	Primary Key(`servicePattermId`),
	Foreign Key(`userId`) References `User`(`userId`)
) Engine=InnoDB Default Charset=utf8;

Create Table `InterfaceMetaData`(
	`interfaceId` int not null auto_increment,
	`serviceId` int not null,
	`interfaceName` varchar(30) not null,
	Primary Key(`interfaceId`),
	Foreign Key(`serviceId`) References `ServiceMetaData`(`serviceId`)
) Engine=InnoDB Default Charset=utf8;

Create Table `InterfacePatterm`(
	`interfacePattermId` int not null auto_increment,
	`servicePattermId` int not null,
	`clientInterfaceId` int not null,
	`serverInterfaceIds` varchar(50) not null,
	Primary Key(`interfacePattermId`),
	Foreign Key(`servicePattermId`) References `ServicePatterm`(`servicePattermId`)
) Engine=InnoDB Default Charset=utf8;

Create Table `TestCase`(
	`testCaseId` int not null auto_increment,
	`interfacePattermId` int not null,
	`clientInterfaceData` varchar(500) not null,
	`serverInterfacesData` varchar(500) not null,
	Primary Key(`testCaseId`),
	Foreign Key(`interfacePattermId`) References `InterfacePatterm`(`interfacePattermId`)
) Engine=InnoDB Default Charset=utf8;
