import { create } from "zustand";
import { useShallow } from "zustand/shallow";

export const useMsgDialogStore = create((set)=>({
  modalState: {open: false, text: null, onClose: null},

  open: ({text, onClose})=> set(()=>({
    modalState: {open: true, text: text, onClose: onClose}
  })),
  
  close: ()=> set(()=>({
    modalState: {open: false, text: null, onClose: null},
  })),
}));

export const useMsgDialogState = ()=> useMsgDialogStore(state => state.modalState);
export const useMsgDialogActions = ()=> useMsgDialogStore(useShallow(state => ({
  openMsgDialog: state.open,
  closeMsgDialog: state.close,
})));