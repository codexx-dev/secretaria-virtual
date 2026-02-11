import {useEffect, useState } from "react";
import { twMerge } from "tailwind-merge";

function SelecableItem({className, select=false, onSelect, children}){
  const [sel, setSel] = useState(select);

  const handleClick = ()=>{
    setSel(!sel);
  };

  useEffect(()=>{
    onSelect?.(sel);
  }, [sel]);

  useEffect(()=>{
    setSel(select);
  }, [select]);

  return(
    <div className={twMerge("px-5 py-1 flex flex-row gap-3 cursor-pointer odd:bg-slate-700 even:bg-slate-600 hover:bg-blue-500 shadow-md", className)}
      onClick={handleClick}
    >
      <input type="checkbox" checked={sel} readOnly/>
      {children}
    </div>
  );
}

export default SelecableItem;