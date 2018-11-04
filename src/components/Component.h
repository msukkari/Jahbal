#pragma once

class BaseEntity;

class Component
{
public:
	Component(BaseEntity* owner) { m_Owner = owner; };

	void SetOwner(BaseEntity* m_Owner) { m_Owner = m_Owner; }
	BaseEntity* GetOwner() { return m_Owner; }

public:
	BaseEntity* m_Owner;
};