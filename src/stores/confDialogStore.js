import { create } from "zustand";
import { useShallow } from "zustand/shallow";

export const useConfDialogStore = create((set)=>({
  modalState: {open: false, text: null, onConfirm: null, oncancel: null, onClose: null},

  open: ({text, onConfirm, oncancel, onClose})=> set(()=>({
    modalState: {open: true, text: text, onConfirm: onConfirm, oncancel: oncancel, onClose: onClose}
  })),
  
  close: ()=> set(()=>({
    modalState: {open: false, text: null, onConfirm: null, onCancel: null},
  })),
}));

export const useConfDialogState = ()=> useConfDialogStore(state => state.modalState);
export const useConfDialogActions = ()=> useConfDialogStore(useShallow(state => ({
  openConfDialog: state.open,
  closeConfDialog: state.close,
})));