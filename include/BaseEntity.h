#pragma once
class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	virtual void Update(float dt) = 0;
};

