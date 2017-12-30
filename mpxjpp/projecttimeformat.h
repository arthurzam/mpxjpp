#ifndef PROJECTTIMEFORMAT_H
#define PROJECTTIMEFORMAT_H

namespace mpxjpp {

enum class ProjectTimeFormat {
	TWELVE_HOUR = 0,
	TWENTY_FOUR_HOUR = 1
};

/**
 * Instances of this class represent enumerated date format values.
 */
enum class ProjectDateFormat {
	DD_MM_YY_HH_MM = 0, ///< This format represents dates in the form 25/12/98 12:56.
	DD_MM_YY, ///< This format represents dates in the form 25/05/98.
	DD_MMMMM_YYYY_HH_MM, ///< This format represents dates in the form 13 December 2002 12:56.
	DD_MMMMM_YYYY, ///< This format represents dates in the form 13 December 2002.
	DD_MMM_HH_MM, ///< This format represents dates in the form 24 Nov 12:56.
	DD_MMM_YY, ///< This format represents dates in the form 25 Aug '98.
	DD_MMMMM, ///< This format represents dates in the form 25 September.
	DD_MMM, ///< This format represents dates in the form 25 Aug.
	EEE_DD_MM_YY_HH_MM, ///< This format represents dates in the form Thu 25/05/98 12:56.
	EEE_DD_MM_YY, ///< This format represents dates in the form Wed 25/05/98.
	EEE_DD_MMM_YY, ///< This format represents dates in the form Wed 25 Mar '98.
	EEE_HH_MM, ///< This format represents dates in the form Wed 12:56.
	DD_MM, ///< This format represents dates in the form 25/5.
	DD, ///< This format represents dates in the form 23.
	HH_MM, ///< This format represents dates in the form 12:56.
	EEE_DD_MMM, ///< This format represents dates in the form Wed 23 Mar.
	EEE_DD_MM, ///< This format represents dates in the form Wed 25/5.
	EEE_DD, ///< This format represents dates in the form Wed 05.
	DD_WWW, ///< This format represents dates in the form 5/W25.
	DD_WWW_YY_HH_MM, ///< This format represents dates in the form 5/W25/98 12:56.
	DD_MM_YYYY ///< This format represents dates in the form 25/05/1998.
};

}

#endif // PROJECTTIMEFORMAT_H
