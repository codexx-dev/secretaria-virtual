import EntryDialog from "../../../components/dialogs/EntryDialog";

function CreateUMEDialog({open, onClose}){

  const textChangeHandle = (ev) => {
    ev.target.value = ev.target.value.toUpperCase();
  }

  return (
    <EntryDialog open={open} onClose={onClose} title={"Adicionar Nova Unidade"}>
      <label>
        Nome da Unidade: 
        <input className="ml-2" onChange={textChangeHandle}/>
      </label>
    </EntryDialog>
  );
}

export default CreateUMEDialog;