import { useState } from "react";

export function useLocalModal(){
  const [flag, setFlag] = useState(false);

  const open = ()=>{
    setFlag(true);
  };

  const close = ()=>{
    setFlag(false);
  };

  const toggle = ()=>{
    setFlag(!flag);
  };

  return [flag, open, close, toggle];
}