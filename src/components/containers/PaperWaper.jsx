import { twMerge } from "tailwind-merge";

function PaperWaper({className, children}){
  return(
    <div className={twMerge("paper-waper" , className)}>
      {children}
    </div>
  );
}

export default PaperWaper;