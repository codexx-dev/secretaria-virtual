import { useMsgDialogActions, useMsgDialogState } from "../../stores/msgDialogStore";
import Modal from "./Modal";

function MessageDialog(){
  const state = useMsgDialogState();
  const {closeMsgDialog} = useMsgDialogActions();

  const handleClose = ()=>{
    state?.onClose();
    closeMsgDialog();
  };

  return (
    <Modal open={state.open}>
      <p className="text-center text-lg">{state.text}</p>

      <div className="grid grid-cols-2 gap-1 px-10">
        <div></div>
        <button className="primary-btn" onClick={handleClose}>OK</button>
      </div>

    </Modal>
  );
}

export default MessageDialog;