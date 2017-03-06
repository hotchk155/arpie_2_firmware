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

#ifndef SOURCES_ARP_INTERFACES_H_
#define SOURCES_ARP_INTERFACES_H_


class IUiComponent {
public:
	virtual void ui_init() =0;
	virtual void ui_done() =0;
	virtual void ui_render(uint32_t *raster, uint32_t *highlight) = 0;
	virtual byte ui_needs_repaint() = 0;
	virtual byte ui_on_key(byte key, byte modifiers) = 0; // return nonzero to say processed
	virtual void ui_on_encoder(char delta, byte modifiers) = 0;
};

class CArpNotes;
class INoteProvider {
public:
	virtual byte get_notes_version() = 0;
	virtual byte get_notes(CArpNotes *notes) = 0;
};

#endif /* SOURCES_ARP_INTERFACES_H_ */
