function Modal({open = true, title, children}){

  return (
    (open) &&
    <div className="absolute top-0 left-0 w-screen h-screen bg-[rgba(0,0,0,0.5)] flex items-center justify-center">
      <div className="mx-15 my-10 py-5 px-10 min-w-120 flex flex-col gap-10 bg-white">
        
        {(title) &&
        <h1 className="text-3xl text-center font-semibold">{title}</h1>}

        {children}

      </div>
    </div>
  );
}

export default Modal;