/*
 * UiParamEdit.h
 *
 *  Created on: 6 Feb 2017
 *      Author: Jason
 */

#ifndef SOURCES_UI_UIPARAMEDIT_H_
#define SOURCES_UI_UIPARAMEDIT_H_

/*
 * Page for editing a numeric parameter
 */
class CUiParamEdit : public CUiPage {
	int m_param;

public:
	CUiParamEdit(int param) {
		m_param = param;
	}
};



#endif /* SOURCES_UI_UIPARAMEDIT_H_ */
