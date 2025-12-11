import Modal from "./Modal";

function MessageDialog({open, title, children, onClose}){

  const closeHandle = ()=>{
    if(onClose) onClose();
  };

  return (
    <Modal open={open} title={title}>

      {children}

      <div className="grid grid-cols-2 gap-1 px-10">
        <div></div>
        <button className="primary-btn" onClick={closeHandle}>OK</button>
      </div>

    </Modal>
  );
}

export default MessageDialog;