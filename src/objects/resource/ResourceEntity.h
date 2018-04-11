#pragma once
#include "database/db_strcut.h"
#include "objects/Entity.h"
#include "objects/Static.h"

class ResourceEntity :
	public Static
{
public:
	ResourceEntity(Vector3* _position, int id, int level);
	virtual ~ResourceEntity();
	int getDbID() override;
	void populate();

	float getMaxHpBarSize() override;
	String& toMultiLineString() override;

	std::string getValues(int precision) override;
	static std::string getColumns();

	float collect(float collectSpeed);
	bool belowLimit();
	void up();
	void reduce();
private:
	db_resource* dbResource;
	int type;
	float amonut;

	int maxUsers;
	int users = 0;
};
