#pragma once

class BaseEntity;

class Component
{
public:
	void SetOwner(BaseEntity* m_Owner) { m_Owner = m_Owner; }
	BaseEntity* GetOwner() { return m_Owner; }

public:
	BaseEntity* m_Owner;
};