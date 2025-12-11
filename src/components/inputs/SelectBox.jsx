function SelectBox({className, children}){
  return(
    <div className={`inline-block relative ${className}`}>
      <button className="w-full py-1 px-2 bg-neutral-50 border-2
      border-neutral-800 rounded-lg after:content-['▼'] after:absolute after:right-3 after:top-1/2 after:-translate-y-1/2">Teste</button>
      <ul className="absolute w-full bg-neutral-50 border border-neutral-500 cursor-default select-none">
        {children}
      </ul>
    </div>
  );
}

export default SelectBox;