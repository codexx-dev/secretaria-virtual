import { useConfDialogActions, useConfDialogState} from "../../stores/confDialogStore";
import EntryDialog from "./EntryDialog";

function ConfDialog(){
  const state = useConfDialogState();
  const {closeConfDialog} = useConfDialogActions();

  const handleClose = ()=>{
    state?.onClose();
    closeConfDialog();
  };

  return(
    <EntryDialog open={state.open} onConfirm={state.onConfirm} onCancel={state.onCancel} onClose={handleClose}>
      <p className="text-center text-lg">{state.text}</p>
    </EntryDialog>
  );
}

export default ConfDialog;
