#ifndef CRITERIAREADER_H
#define CRITERIAREADER_H

#include <vector>
#include <cstdint>

namespace mpxjpp {

class FieldType;

namespace mpp {

class CriteriaReader
{
protected:
	virtual int getCriteriaStartOffset() = 0;
	virtual int getCriteriaBlockSize() = 0;
	virtual std::vector<uint8_t> getChildBlock(std::vector<uint8_t> block) = 0;
	virtual std::vector<uint8_t> getListNextBlock(std::vector<uint8_t> block) = 0;
	virtual int getTextOffset(std::vector<uint8_t> block) = 0;
	virtual int getPromptOffset(std::vector<uint8_t> block) = 0;
	virtual int getValueOffset() = 0;
	virtual int getTimeUnitsOffset() = 0;
	virtual int getCriteriaTextStartOffset() = 0;
	virtual FieldType getFieldType(std::vector<uint8_t> block) = 0;
public:
	CriteriaReader();
};

}
}
#endif // CRITERIAREADER_H
