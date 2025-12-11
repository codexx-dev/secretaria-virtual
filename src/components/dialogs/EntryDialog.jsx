import Modal from "./Modal";

function EntryDialog({open, title, children, onConfirm, onCancel, onClose}){
  const confirmHandle = ()=>{
    if(onConfirm) onConfirm();
    if(onClose) onClose();
  };

  const cancelHandle = ()=>{
    if(onCancel) onCancel();
    if(onClose) onClose();
  };

  return (
    <Modal open={open} title={title}>
      
      {children}

      <div className="grid grid-cols-2 gap-5 px-10">
        <button className="sucess-btn" onClick={confirmHandle}>Confirmar</button>
        <button className="primary-btn" onClick={cancelHandle}>Cancelar</button>
      </div>

    </Modal>
  );
}

export default EntryDialog;