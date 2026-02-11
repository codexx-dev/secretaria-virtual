import EntryDialog from "../../../../components/dialogs/EntryDialog";
import MonthSelect from "../inputs/MothSelect";

function CreateBookDialog({open=true, bookTitle, refYear, onConfirm, onClose}){
  const refMonth = 'testOK'
  const bookName = 'testOK'
  const useLastFalg = true;

  const handleConfirm = ()=>{
    onConfirm({bookName, refMonth, useLastFalg});
  }

  return (
    <EntryDialog open={open} title={"CRIAR LIVRO PONTO/" + refYear} onClose={onClose} onConfirm={handleConfirm}>
      <div className="flex flex-col gap-2 px-12">
        <label className="text-lg">
          Título:
          <input className="ml-1 w-full" value={bookTitle}></input>
        </label>

        <div className="flex flex-row gap-2">
        <label className="text-l ">
          Unidade Administrativa / Escolar:
          <br/>
          <select className="w-full">
            <option>UME Monte Cabrão</option>
            <option>UME Ilha Diana</option>
            <option>Inserir Novo Nome</option>
          </select>
        </label>
        <input className="ml-1 grow self-baseline-last" disabled></input>
        </div>

        <div className="mb-5 mx-10 flex flex-row justify-around gap-5">
          <label className="text-lg font-normal whitespace-nowrap">
            Mês de referência:
            <MonthSelect/>
          </label>
          
          <label className="text-lg font-normal whitespace-nowrap">
            Usar livro anterior como modelo:
            <input className="scale-110 translate-y-[1.5px] ml-2" type="checkbox"></input>
          </label>
        </div>

      </div>
    </EntryDialog>
  );
}

export default CreateBookDialog;