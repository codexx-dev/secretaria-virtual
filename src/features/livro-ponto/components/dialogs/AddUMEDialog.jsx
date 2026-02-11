import EntryDialog from "../../../../components/dialogs/EntryDialog";

function AddUMEDialog({open, onClose}){

  const handleTextChange = (ev) => {
    ev.target.value = ev.target.value.toUpperCase();
  }

  return (
    <EntryDialog open={open} onClose={onClose} title={"Adicionar Nova Unidade"}>
      <label>
        Nome da Unidade: 
        <input className="ml-2" onChange={handleTextChange}/>
      </label>
    </EntryDialog>
  );
}

export default AddUMEDialog;