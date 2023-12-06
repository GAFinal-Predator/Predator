#pragma once

#include "IResource.h"

#include <string>

class Resource : virtual public yunuGI::IResource
{
public:
	Resource();

public:
	static unsigned int staticID;

protected:
	unsigned int id;

public:
	// IResource��(��) ���� ��ӵ�
	virtual const std::wstring& GetName() const override;

	virtual void SetName(const std::wstring& name) override;

	virtual unsigned int GetID() override;

};


