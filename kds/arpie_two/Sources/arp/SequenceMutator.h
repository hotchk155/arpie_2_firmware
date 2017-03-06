////////////////////////////////////////////////////////
//
//                                           //// ////
//      ////     ////  ////    //  ////      //   //
//         //  //     //  //      //  //    //   //
//     /////  //     //  //  //  //////    //   //
//   //  //  //     //  //  //  //        //   //
//   /////  //     /////   //   //////  //// ////
//                //
//               //     MIDI/CV ARPEGGIATOR
//                        2017/hotchk155
//
////////////////////////////////////////////////////////

#ifndef SOURCES_SEQUENCEMUTATOR_H_
#define SOURCES_SEQUENCEMUTATOR_H_

class CSequenceMutator {
public:
	typedef struct {
		byte m_mode;
	} CONFIG;
	CONFIG m_cfg;
	CSequenceMutator() {
	}
	void init() {
		m_cfg.m_mode = params::SEQ_MUTATE_NONE;
	}
	void process(CArpNotes *seq) {
		switch(m_cfg.m_mode) {
		case params::SEQ_MUTATE_INSERT_HIGHEST:
			seq->insert_note(seq->highest(), 0);
			break;
		case params::SEQ_MUTATE_INSERT_LOWEST:
			seq->insert_note(seq->lowest(), 0);
			break;
		case params::SEQ_MUTATE_3F1B:
			seq->mutate(3,1);
			break;
		case params::SEQ_MUTATE_4F2B:
			seq->mutate(4,2);
			break;
		}
	}
};



#endif /* SOURCES_SEQUENCEMUTATOR_H_ */
