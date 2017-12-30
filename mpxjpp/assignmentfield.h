#ifndef ASSIGNMENTFIELD_H
#define ASSIGNMENTFIELD_H

#include "fieldtype.h"

namespace mpxjpp {

struct AssignmentField final : public FieldType {

#define ASSIGNMENT_FIELD_GEN(F) \
	F(START, DataType::DATE),\
	\
	F(DURATION1_UNITS, DataType::TIME_UNITS),\
	F(DURATION2_UNITS, DataType::TIME_UNITS),\
	F(DURATION3_UNITS, DataType::TIME_UNITS),\
	F(DURATION4_UNITS, DataType::TIME_UNITS),\
	F(DURATION5_UNITS, DataType::TIME_UNITS),\
	F(DURATION6_UNITS, DataType::TIME_UNITS),\
	F(DURATION7_UNITS, DataType::TIME_UNITS),\
	F(DURATION8_UNITS, DataType::TIME_UNITS),\
	F(DURATION9_UNITS, DataType::TIME_UNITS),\
	F(DURATION10_UNITS, DataType::TIME_UNITS),\
	F(ACTUAL_COST, DataType::CURRENCY),\
	F(ACTUAL_WORK, DataType::WORK),\
	F(COST, DataType::CURRENCY),\
	F(ASSIGNMENT_DELAY, DataType::DELAY),\
	F(VARIABLE_RATE_UNITS, DataType::WORK_UNITS),\
	F(ASSIGNMENT_UNITS, DataType::UNITS),\
	F(WORK, DataType::WORK),\
	F(BASELINE_START, DataType::DATE),\
	F(ACTUAL_START, DataType::DATE),\
	F(BASELINE_FINISH, DataType::DATE),\
	F(ACTUAL_FINISH, DataType::DATE),\
	F(BASELINE_WORK, DataType::WORK),\
	F(OVERTIME_WORK, DataType::WORK),\
	F(BASELINE_COST, DataType::CURRENCY),\
	F(WORK_CONTOUR, DataType::STRING),\
	F(REMAINING_WORK, DataType::WORK),\
	F(LEVELING_DELAY_UNITS, DataType::TIME_UNITS),\
	F(LEVELING_DELAY, DataType::DURATION, LEVELING_DELAY_UNITS),\
	F(UNIQUE_ID, DataType::INTEGER),\
	F(TASK_UNIQUE_ID, DataType::INTEGER),\
	F(RESOURCE_UNIQUE_ID, DataType::INTEGER),\
	F(TIMEPHASED_WORK, DataType::BINARY),\
	F(TIMEPHASED_ACTUAL_WORK, DataType::BINARY),\
	F(TIMEPHASED_ACTUAL_OVERTIME_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE1_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE2_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE3_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE4_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE5_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE6_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE7_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE8_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE9_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE10_WORK, DataType::BINARY),\
	F(TIMEPHASED_BASELINE_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE1_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE2_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE3_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE4_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE5_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE6_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE7_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE8_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE9_COST, DataType::BINARY),\
	F(TIMEPHASED_BASELINE10_COST, DataType::BINARY),\
	F(TASK_ID, DataType::INTEGER),\
	F(RESOURCE_ID, DataType::INTEGER),\
	F(TASK_NAME, DataType::STRING),\
	F(RESOURCE_NAME, DataType::STRING),\
	F(REGULAR_WORK, DataType::WORK),\
	F(ACTUAL_OVERTIME_WORK, DataType::WORK),\
	F(REMAINING_OVERTIME_WORK, DataType::WORK),\
	F(PEAK, DataType::UNITS),\
	F(OVERTIME_COST, DataType::CURRENCY),\
	F(REMAINING_COST, DataType::CURRENCY),\
	F(ACTUAL_OVERTIME_COST, DataType::CURRENCY),\
	F(REMAINING_OVERTIME_COST, DataType::CURRENCY),\
	F(BCWS, DataType::CURRENCY),\
	F(BCWP, DataType::CURRENCY),\
	F(ACWP, DataType::CURRENCY),\
	F(SV, DataType::CURRENCY),\
	F(COST_VARIANCE, DataType::CURRENCY),\
	F(PERCENT_WORK_COMPLETE, DataType::PERCENTAGE),\
	F(PROJECT, DataType::STRING),\
	F(NOTES, DataType::ASCII_STRING),\
	F(CONFIRMED, DataType::BOOLEAN),\
	F(RESPONSE_PENDING, DataType::BOOLEAN),\
	F(UPDATE_NEEDED, DataType::BOOLEAN),\
	F(TEAM_STATUS_PENDING, DataType::BOOLEAN),\
	F(COST_RATE_TABLE, DataType::SHORT),\
	F(TEXT1, DataType::STRING),\
	F(TEXT2, DataType::STRING),\
	F(TEXT3, DataType::STRING),\
	F(TEXT4, DataType::STRING),\
	F(TEXT5, DataType::STRING),\
	F(TEXT6, DataType::STRING),\
	F(TEXT7, DataType::STRING),\
	F(TEXT8, DataType::STRING),\
	F(TEXT9, DataType::STRING),\
	F(TEXT10, DataType::STRING),\
	F(START1, DataType::DATE),\
	F(START2, DataType::DATE),\
	F(START3, DataType::DATE),\
	F(START4, DataType::DATE),\
	F(START5, DataType::DATE),\
	F(FINISH1, DataType::DATE),\
	F(FINISH2, DataType::DATE),\
	F(FINISH3, DataType::DATE),\
	F(FINISH4, DataType::DATE),\
	F(FINISH5, DataType::DATE),\
	F(NUMBER1, DataType::NUMERIC),\
	F(NUMBER2, DataType::NUMERIC),\
	F(NUMBER3, DataType::NUMERIC),\
	F(NUMBER4, DataType::NUMERIC),\
	F(NUMBER5, DataType::NUMERIC),\
	F(DURATION1, DataType::DURATION, DURATION1_UNITS),\
	F(DURATION2, DataType::DURATION, DURATION2_UNITS),\
	F(DURATION3, DataType::DURATION, DURATION3_UNITS),\
	F(COST1, DataType::CURRENCY),\
	F(COST2, DataType::CURRENCY),\
	F(COST3, DataType::CURRENCY),\
	F(FLAG10, DataType::BOOLEAN),\
	F(FLAG1, DataType::BOOLEAN),\
	F(FLAG2, DataType::BOOLEAN),\
	F(FLAG3, DataType::BOOLEAN),\
	F(FLAG4, DataType::BOOLEAN),\
	F(FLAG5, DataType::BOOLEAN),\
	F(FLAG6, DataType::BOOLEAN),\
	F(FLAG7, DataType::BOOLEAN),\
	F(FLAG8, DataType::BOOLEAN),\
	F(FLAG9, DataType::BOOLEAN),\
	F(LINKED_FIELDS, DataType::BOOLEAN),\
	F(OVERALLOCATED, DataType::BOOLEAN),\
	F(TASK_SUMMARY_NAME, DataType::STRING),\
	F(HYPERLINK, DataType::STRING),\
	F(HYPERLINK_ADDRESS, DataType::STRING),\
	F(HYPERLINK_SUBADDRESS, DataType::STRING),\
	F(HYPERLINK_HREF, DataType::STRING),\
	F(COST4, DataType::CURRENCY),\
	F(COST5, DataType::CURRENCY),\
	F(COST6, DataType::CURRENCY),\
	F(COST7, DataType::CURRENCY),\
	F(COST8, DataType::CURRENCY),\
	F(COST9, DataType::CURRENCY),\
	F(COST10, DataType::CURRENCY),\
	F(DATE1, DataType::DATE),\
	F(DATE2, DataType::DATE),\
	F(DATE3, DataType::DATE),\
	F(DATE4, DataType::DATE),\
	F(DATE5, DataType::DATE),\
	F(DATE6, DataType::DATE),\
	F(DATE7, DataType::DATE),\
	F(DATE8, DataType::DATE),\
	F(DATE9, DataType::DATE),\
	F(DATE10, DataType::DATE),\
	F(DURATION4, DataType::DURATION, DURATION4_UNITS),\
	F(DURATION5, DataType::DURATION, DURATION5_UNITS),\
	F(DURATION6, DataType::DURATION, DURATION6_UNITS),\
	F(DURATION7, DataType::DURATION, DURATION7_UNITS),\
	F(DURATION8, DataType::DURATION, DURATION8_UNITS),\
	F(DURATION9, DataType::DURATION, DURATION9_UNITS),\
	F(DURATION10, DataType::DURATION, DURATION10_UNITS),\
	F(FINISH6, DataType::DATE),\
	F(FINISH7, DataType::DATE),\
	F(FINISH8, DataType::DATE),\
	F(FINISH9, DataType::DATE),\
	F(FINISH10, DataType::DATE),\
	F(FLAG11, DataType::BOOLEAN),\
	F(FLAG12, DataType::BOOLEAN),\
	F(FLAG13, DataType::BOOLEAN),\
	F(FLAG14, DataType::BOOLEAN),\
	F(FLAG15, DataType::BOOLEAN),\
	F(FLAG16, DataType::BOOLEAN),\
	F(FLAG17, DataType::BOOLEAN),\
	F(FLAG18, DataType::BOOLEAN),\
	F(FLAG19, DataType::BOOLEAN),\
	F(FLAG20, DataType::BOOLEAN),\
	F(NUMBER6, DataType::NUMERIC),\
	F(NUMBER7, DataType::NUMERIC),\
	F(NUMBER8, DataType::NUMERIC),\
	F(NUMBER9, DataType::NUMERIC),\
	F(NUMBER10, DataType::NUMERIC),\
	F(NUMBER11, DataType::NUMERIC),\
	F(NUMBER12, DataType::NUMERIC),\
	F(NUMBER13, DataType::NUMERIC),\
	F(NUMBER14, DataType::NUMERIC),\
	F(NUMBER15, DataType::NUMERIC),\
	F(NUMBER16, DataType::NUMERIC),\
	F(NUMBER17, DataType::NUMERIC),\
	F(NUMBER18, DataType::NUMERIC),\
	F(NUMBER19, DataType::NUMERIC),\
	F(NUMBER20, DataType::NUMERIC),\
	F(START6, DataType::DATE),\
	F(START7, DataType::DATE),\
	F(START8, DataType::DATE),\
	F(START9, DataType::DATE),\
	F(START10, DataType::DATE),\
	F(TEXT11, DataType::STRING),\
	F(TEXT12, DataType::STRING),\
	F(TEXT13, DataType::STRING),\
	F(TEXT14, DataType::STRING),\
	F(TEXT15, DataType::STRING),\
	F(TEXT16, DataType::STRING),\
	F(TEXT17, DataType::STRING),\
	F(TEXT18, DataType::STRING),\
	F(TEXT19, DataType::STRING),\
	F(TEXT20, DataType::STRING),\
	F(TEXT21, DataType::STRING),\
	F(TEXT22, DataType::STRING),\
	F(TEXT23, DataType::STRING),\
	F(TEXT24, DataType::STRING),\
	F(TEXT25, DataType::STRING),\
	F(TEXT26, DataType::STRING),\
	F(TEXT27, DataType::STRING),\
	F(TEXT28, DataType::STRING),\
	F(TEXT29, DataType::STRING),\
	F(TEXT30, DataType::STRING),\
	F(INDEX, DataType::INTEGER),\
	F(CV, DataType::CURRENCY),\
	F(WORK_VARIANCE, DataType::WORK),\
	F(START_VARIANCE, DataType::DURATION),\
	F(FINISH_VARIANCE, DataType::DURATION),\
	F(VAC, DataType::CURRENCY),\
	F(FIXED_MATERIAL_ASSIGNMENT, DataType::STRING),\
	F(RESOURCE_TYPE, DataType::RESOURCE_TYPE),\
	F(HYPERLINK_SCREEN_TIP, DataType::STRING),\
	F(WBS, DataType::STRING),\
	F(BASELINE1_WORK, DataType::WORK),\
	F(BASELINE1_COST, DataType::CURRENCY),\
	F(BASELINE1_START, DataType::DATE),\
	F(BASELINE1_FINISH, DataType::DATE),\
	F(BASELINE2_WORK, DataType::WORK),\
	F(BASELINE2_COST, DataType::CURRENCY),\
	F(BASELINE2_START, DataType::DATE),\
	F(BASELINE2_FINISH, DataType::DATE),\
	F(BASELINE3_WORK, DataType::WORK),\
	F(BASELINE3_COST, DataType::CURRENCY),\
	F(BASELINE3_START, DataType::DATE),\
	F(BASELINE3_FINISH, DataType::DATE),\
	F(BASELINE4_WORK, DataType::WORK),\
	F(BASELINE4_COST, DataType::CURRENCY),\
	F(BASELINE4_START, DataType::DATE),\
	F(BASELINE4_FINISH, DataType::DATE),\
	F(BASELINE5_WORK, DataType::WORK),\
	F(BASELINE5_COST, DataType::CURRENCY),\
	F(BASELINE5_START, DataType::DATE),\
	F(BASELINE5_FINISH, DataType::DATE),\
	F(BASELINE6_WORK, DataType::WORK),\
	F(BASELINE6_COST, DataType::CURRENCY),\
	F(BASELINE6_START, DataType::DATE),\
	F(BASELINE6_FINISH, DataType::DATE),\
	F(BASELINE7_WORK, DataType::WORK),\
	F(BASELINE7_COST, DataType::CURRENCY),\
	F(BASELINE7_START, DataType::DATE),\
	F(BASELINE7_FINISH, DataType::DATE),\
	F(BASELINE8_WORK, DataType::WORK),\
	F(BASELINE8_COST, DataType::CURRENCY),\
	F(BASELINE8_START, DataType::DATE),\
	F(BASELINE8_FINISH, DataType::DATE),\
	F(BASELINE9_WORK, DataType::WORK),\
	F(BASELINE9_COST, DataType::CURRENCY),\
	F(BASELINE9_START, DataType::DATE),\
	F(BASELINE9_FINISH, DataType::DATE),\
	F(BASELINE10_WORK, DataType::WORK),\
	F(BASELINE10_COST, DataType::CURRENCY),\
	F(BASELINE10_START, DataType::DATE),\
	F(BASELINE10_FINISH, DataType::DATE),\
	F(TASK_OUTLINE_NUMBER, DataType::STRING),\
	F(ENTERPRISE_COST1, DataType::CURRENCY),\
	F(ENTERPRISE_COST2, DataType::CURRENCY),\
	F(ENTERPRISE_COST3, DataType::CURRENCY),\
	F(ENTERPRISE_COST4, DataType::CURRENCY),\
	F(ENTERPRISE_COST5, DataType::CURRENCY),\
	F(ENTERPRISE_COST6, DataType::CURRENCY),\
	F(ENTERPRISE_COST7, DataType::CURRENCY),\
	F(ENTERPRISE_COST8, DataType::CURRENCY),\
	F(ENTERPRISE_COST9, DataType::CURRENCY),\
	F(ENTERPRISE_COST10, DataType::CURRENCY),\
	F(ENTERPRISE_DATE1, DataType::DATE),\
	F(ENTERPRISE_DATE2, DataType::DATE),\
	F(ENTERPRISE_DATE3, DataType::DATE),\
	F(ENTERPRISE_DATE4, DataType::DATE),\
	F(ENTERPRISE_DATE5, DataType::DATE),\
	F(ENTERPRISE_DATE6, DataType::DATE),\
	F(ENTERPRISE_DATE7, DataType::DATE),\
	F(ENTERPRISE_DATE8, DataType::DATE),\
	F(ENTERPRISE_DATE9, DataType::DATE),\
	F(ENTERPRISE_DATE10, DataType::DATE),\
	F(ENTERPRISE_DATE11, DataType::DATE),\
	F(ENTERPRISE_DATE12, DataType::DATE),\
	F(ENTERPRISE_DATE13, DataType::DATE),\
	F(ENTERPRISE_DATE14, DataType::DATE),\
	F(ENTERPRISE_DATE15, DataType::DATE),\
	F(ENTERPRISE_DATE16, DataType::DATE),\
	F(ENTERPRISE_DATE17, DataType::DATE),\
	F(ENTERPRISE_DATE18, DataType::DATE),\
	F(ENTERPRISE_DATE19, DataType::DATE),\
	F(ENTERPRISE_DATE20, DataType::DATE),\
	F(ENTERPRISE_DATE21, DataType::DATE),\
	F(ENTERPRISE_DATE22, DataType::DATE),\
	F(ENTERPRISE_DATE23, DataType::DATE),\
	F(ENTERPRISE_DATE24, DataType::DATE),\
	F(ENTERPRISE_DATE25, DataType::DATE),\
	F(ENTERPRISE_DATE26, DataType::DATE),\
	F(ENTERPRISE_DATE27, DataType::DATE),\
	F(ENTERPRISE_DATE28, DataType::DATE),\
	F(ENTERPRISE_DATE29, DataType::DATE),\
	F(ENTERPRISE_DATE30, DataType::DATE),\
	F(ENTERPRISE_DURATION1, DataType::DURATION),\
	F(ENTERPRISE_DURATION2, DataType::DURATION),\
	F(ENTERPRISE_DURATION3, DataType::DURATION),\
	F(ENTERPRISE_DURATION4, DataType::DURATION),\
	F(ENTERPRISE_DURATION5, DataType::DURATION),\
	F(ENTERPRISE_DURATION6, DataType::DURATION),\
	F(ENTERPRISE_DURATION7, DataType::DURATION),\
	F(ENTERPRISE_DURATION8, DataType::DURATION),\
	F(ENTERPRISE_DURATION9, DataType::DURATION),\
	F(ENTERPRISE_DURATION10, DataType::DURATION),\
	F(ENTERPRISE_FLAG1, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG2, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG3, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG4, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG5, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG6, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG7, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG8, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG9, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG10, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG11, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG12, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG13, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG14, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG15, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG16, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG17, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG18, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG19, DataType::BOOLEAN),\
	F(ENTERPRISE_FLAG20, DataType::BOOLEAN),\
	F(ENTERPRISE_NUMBER1, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER2, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER3, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER4, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER5, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER6, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER7, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER8, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER9, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER10, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER11, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER12, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER13, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER14, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER15, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER16, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER17, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER18, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER19, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER20, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER21, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER22, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER23, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER24, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER25, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER26, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER27, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER28, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER29, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER30, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER31, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER32, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER33, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER34, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER35, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER36, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER37, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER38, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER39, DataType::NUMERIC),\
	F(ENTERPRISE_NUMBER40, DataType::NUMERIC),\
	F(ENTERPRISE_TEXT1, DataType::STRING),\
	F(ENTERPRISE_TEXT2, DataType::STRING),\
	F(ENTERPRISE_TEXT3, DataType::STRING),\
	F(ENTERPRISE_TEXT4, DataType::STRING),\
	F(ENTERPRISE_TEXT5, DataType::STRING),\
	F(ENTERPRISE_TEXT6, DataType::STRING),\
	F(ENTERPRISE_TEXT7, DataType::STRING),\
	F(ENTERPRISE_TEXT8, DataType::STRING),\
	F(ENTERPRISE_TEXT9, DataType::STRING),\
	F(ENTERPRISE_TEXT10, DataType::STRING),\
	F(ENTERPRISE_TEXT11, DataType::STRING),\
	F(ENTERPRISE_TEXT12, DataType::STRING),\
	F(ENTERPRISE_TEXT13, DataType::STRING),\
	F(ENTERPRISE_TEXT14, DataType::STRING),\
	F(ENTERPRISE_TEXT15, DataType::STRING),\
	F(ENTERPRISE_TEXT16, DataType::STRING),\
	F(ENTERPRISE_TEXT17, DataType::STRING),\
	F(ENTERPRISE_TEXT18, DataType::STRING),\
	F(ENTERPRISE_TEXT19, DataType::STRING),\
	F(ENTERPRISE_TEXT20, DataType::STRING),\
	F(ENTERPRISE_TEXT21, DataType::STRING),\
	F(ENTERPRISE_TEXT22, DataType::STRING),\
	F(ENTERPRISE_TEXT23, DataType::STRING),\
	F(ENTERPRISE_TEXT24, DataType::STRING),\
	F(ENTERPRISE_TEXT25, DataType::STRING),\
	F(ENTERPRISE_TEXT26, DataType::STRING),\
	F(ENTERPRISE_TEXT27, DataType::STRING),\
	F(ENTERPRISE_TEXT28, DataType::STRING),\
	F(ENTERPRISE_TEXT29, DataType::STRING),\
	F(ENTERPRISE_TEXT30, DataType::STRING),\
	F(ENTERPRISE_TEXT31, DataType::STRING),\
	F(ENTERPRISE_TEXT32, DataType::STRING),\
	F(ENTERPRISE_TEXT33, DataType::STRING),\
	F(ENTERPRISE_TEXT34, DataType::STRING),\
	F(ENTERPRISE_TEXT35, DataType::STRING),\
	F(ENTERPRISE_TEXT36, DataType::STRING),\
	F(ENTERPRISE_TEXT37, DataType::STRING),\
	F(ENTERPRISE_TEXT38, DataType::STRING),\
	F(ENTERPRISE_TEXT39, DataType::STRING),\
	F(ENTERPRISE_TEXT40, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE1, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE2, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE3, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE4, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE5, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE6, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE7, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE8, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE9, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE10, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE11, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE12, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE13, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE14, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE15, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE16, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE17, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE18, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE19, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE20, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE21, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE22, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE23, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE24, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE25, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE26, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE27, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE28, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_OUTLINE_CODE29, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_RBS, DataType::STRING),\
	F(RESOURCE_REQUEST_TYPE, DataType::RESOURCE_REQUEST_TYPE),\
	F(ENTERPRISE_TEAM_MEMBER, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE20, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE21, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE22, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE23, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE24, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE25, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE26, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE27, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE28, DataType::STRING),\
	F(ENTERPRISE_RESOURCE_MULTI_VALUE29, DataType::STRING),\
	F(ACTUAL_WORK_PROTECTED, DataType::WORK),\
	F(ACTUAL_OVERTIME_WORK_PROTECTED, DataType::WORK),\
	F(CREATED, DataType::DATE),\
	F(GUID, DataType::GUID),\
	F(ASSIGNMENT_TASK_GUID, DataType::GUID),\
	F(ASSIGNMENT_RESOURCE_GUID, DataType::GUID),\
	F(SUMMARY, DataType::STRING),\
	F(OWNER, DataType::STRING),\
	F(BUDGET_WORK, DataType::WORK),\
	F(BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE_BUDGET_WORK, DataType::WORK),\
	F(BASELINE_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE1_BUDGET_WORK, DataType::WORK),\
	F(BASELINE1_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE2_BUDGET_WORK, DataType::WORK),\
	F(BASELINE2_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE3_BUDGET_WORK, DataType::WORK),\
	F(BASELINE3_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE4_BUDGET_WORK, DataType::WORK),\
	F(BASELINE4_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE5_BUDGET_WORK, DataType::WORK),\
	F(BASELINE5_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE6_BUDGET_WORK, DataType::WORK),\
	F(BASELINE6_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE7_BUDGET_WORK, DataType::WORK),\
	F(BASELINE7_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE8_BUDGET_WORK, DataType::WORK),\
	F(BASELINE8_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE9_BUDGET_WORK, DataType::WORK),\
	F(BASELINE9_BUDGET_COST, DataType::CURRENCY),\
	F(BASELINE10_BUDGET_WORK, DataType::WORK),\
	F(BASELINE10_BUDGET_COST, DataType::CURRENCY),\
	F(UNAVAILABLE, DataType::STRING),\
	F(HYPERLINK_DATA, DataType::BINARY),\
	F(RESUME, DataType::DATE),\
	F(STOP, DataType::DATE),\
	\
	F(ENTERPRISE_CUSTOM_FIELD1, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD2, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD3, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD4, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD5, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD6, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD7, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD8, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD9, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD10, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD11, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD12, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD13, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD14, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD15, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD16, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD17, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD18, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD19, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD20, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD21, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD22, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD23, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD24, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD25, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD26, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD27, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD28, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD29, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD30, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD31, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD32, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD33, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD34, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD35, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD36, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD37, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD38, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD39, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD40, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD41, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD42, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD43, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD44, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD45, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD46, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD47, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD48, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD49, DataType::STRING),\
	F(ENTERPRISE_CUSTOM_FIELD50, DataType::STRING),\
	\
	F(FINISH, DataType::DATE)

public:
	enum Fields {
		ASSIGNMENT_FIELD_GEN(MPXJPP_GEN_ENUM)
	};
private:
	static struct FieldType::priv_field_t g_fields[];
	static struct FieldType::priv_arr_t g_class;
public:
	constexpr AssignmentField(int val) :
		FieldType(val, &g_class)
	{}
};

}

#endif // ASSIGNMENTFIELD_H
