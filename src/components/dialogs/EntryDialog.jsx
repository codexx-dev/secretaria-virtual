import Modal from "./Modal";

function EntryDialog({open, title, children, onConfirm, onCancel, onClose}){
  const handleConfirm = ()=>{
    onConfirm?.();
  };

  const handleCancel = ()=>{
    onCancel?.();
    onClose?.();
  };

  return (
    <Modal open={open} title={title}>
      
      {children}

      <div className="grid grid-cols-2 gap-5 px-10">
        <button className="sucess-btn" onClick={handleConfirm}>Confirmar</button>
        <button className="primary-btn" onClick={handleCancel}>Cancelar</button>
      </div>

    </Modal>
  );
}

export default EntryDialog;