import React, { useState } from "react";
import { twMerge } from "tailwind-merge";

function SelectableList({className, children}){
  const [selAll, setSelAll] = useState(false);

  const handleSelectAll = ()=>{
    setSelAll(!selAll);
  }

  return(
    <div className={twMerge("flex flex-col bg-slate-700 text-neutral-50 overflow-y-scroll", className)}>
      <div className="gap-1 h-10 px-5 flex items-center bg-slate-700">
        <input id="sel-all" type="checkbox" onChange={handleSelectAll} />
        <label htmlFor="sel-all">Selecionar Todos</label>
      </div>
      {React.Children.map(children, (child)=>{
        if (!React.isValidElement(child)) return child;
        return React.cloneElement(child, {select: selAll});
      })}
    </div>
  );
}

export default SelectableList;