#The Following Lines are generated by the gentity tool 
#based on the input given in the entitytypes.xml file




testentity4_testentity = db.table("testentity4_testentity",
	db.Column("testentity_id", db.Integer, db.ForeignKey("testentity.id")),
	db.Column("testentity4_id", db.Integer, db.ForeignKey("testentity4.id")))


# testdesc
class Testentity(db.Model):
	__tablename__ = testentity
	#attrdesc1
	attr1 = db.Column(db.Integer) 
	#attrdesc2
	attr2 = db.Column(db.Integer) 
	testentity1 = db.relationship("Testentity1", backref="testentity" ,uselist=False)
	testentity3s = db.relationship("Testentity3", backref="testentity")
	testentity4s = db.relationship("Testentity4", secondary=testentity4_testentity, backref=db.backrefs("testentity4_testentity") )



# testdesc1
class Testentity1(db.Model):
	__tablename__ = testentity1
	#attrdesc11
	attr11 = db.Column(db.Integer) 
	#attrdesc21
	attr21 = db.Column(db.Integer) 
	testentity_id = db.Column(db.Integer,db.ForeignKey('testentity.id'), unique=True)



# testdesc3
class Testentity3(db.Model):
	__tablename__ = testentity3
	#attrdesc112
	attr121 = db.Column(db.Integer) 
	#attrdesc221
	attr212 = db.Column(db.Integer) 
	testentity_id = db.Column(db.Integer,db.ForeignKey('testentity.id'))



# testdesc4
class Testentity4(db.Model):
	__tablename__ = testentity4
	#attrdesc112
	attr121 = db.Column(db.Integer) 
	#attrdesc221
	attr212 = db.Column(db.Integer) 


