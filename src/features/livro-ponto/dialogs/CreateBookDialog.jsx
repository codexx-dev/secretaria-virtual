import EntryDialog from "../../../components/dialogs/EntryDialog";
import { getMonthName } from "../../../utils";

function CreateBookDialog({open=true, onClose}){
  return (
    <EntryDialog open={open} title={"LIVRO PONTO/2025"} onClose={onClose}>
      <div className="flex flex-col gap-2">
        <label className="text-lg font-normal">
          Mês de referência:
          <select className="ml-1 text-black">
            {Array.from({length:12}).map((_, idx)=>{
              const month1 = getMonthName(idx + 1).substring(0, 3).toLocaleUpperCase();
              const month2 = getMonthName(idx + 2).substring(0, 3).toLocaleUpperCase();
              return <option>{`${month1}/${month2}`}</option>;
            })}
          </select>
        </label>
        
        <label className="text-lg font-normal">
          Usar livro anterior como modelo:
          <input className="scale-110 translate-y-[1.5px] ml-2" type="checkbox"></input>
        </label>
      </div>
    </EntryDialog>
  );
}

export default CreateBookDialog;