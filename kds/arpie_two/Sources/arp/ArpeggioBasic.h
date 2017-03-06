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

#ifndef SOURCES_ARPEGGIOBASIC_H_
#define SOURCES_ARPEGGIOBASIC_H_

class CArpeggioBasic {
public:

	typedef struct {
		byte arp_type;
		byte octaves;
	} CONFIG;
	CONFIG m_cfg;

	CArpeggioBasic() {
	}

	void init() {
		m_cfg.arp_type = params::ARPTYPE_UP;
		m_cfg.octaves = 3;
	}

	///////////////////////////////////////////////////////////////
	void process(CArpNotes *notes) {
		notes->extend(m_cfg.octaves);
		switch(m_cfg.arp_type) {
			case params::ARPTYPE_UP:
				notes->sort(1);
				break;
			case params::ARPTYPE_DN:
				notes->sort(0);
				break;
			case params::ARPTYPE_UPDN:
				notes->sort(1);
				notes->reflect();
				break;
			case params::ARPTYPE_DNUP:
				notes->sort(1);
				notes->reflect();
				break;
			case params::ARPTYPE_RND:
				notes->shuffle();
				break;
			case params::ARPTYPE_CHORD:
				notes->to_chord();
				break;
			default:
				break;
		}
	}


};



#endif /* SOURCES_ARPEGGIOBASIC_H_ */
