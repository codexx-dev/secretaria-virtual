import { useEffect, useRef, useState } from "react";
import SelecableItem from "../../components/containers/SelecableItem";
import SelectableList from "../../components/containers/SelectableList";
import { useLocalModal } from "../../hooks/useLocalModal";
import CrachaInfoDialog from "./components/dialogs/CrachaInfoDialog";
import { useCrachaActions, useCrachaList } from "./stores/crachaStore";
import { BASENAME } from "../../routes";

function CrachaCreator(){
  const crachaList = useCrachaList();
  const {updateList, addCracha, delCracha, updateCracha} = useCrachaActions();
  const [createFlag, openCreateDialog, closeCreateDialog] = useLocalModal();
  const [editFlag, openEditDialog, closeEditDialog] = useLocalModal();
  const [query, setQuery] = useState({
    ume: 'UME Monte Cabrão',
    refType: 'regular',
    refYear: '2026',
    refClass: 'MII-A',
  });
  const [editInfo, setEditInfo] = useState(null);
  const selectedItens = useRef({});

  const haldleCreate = async(info)=>{
    await addCracha({...query, ...info});
    closeCreateDialog();
  };
  
  const handleDelete = ()=>{
    //debug
    //console.log(selectedItens.current);

    crachaList.forEach((craha)=>{
      if(selectedItens.current[craha.id]){
        delCracha(craha.id);
        delete selectedItens.current[craha.id];
      }
    });
  };
  
  const handleEdit = (e, info)=>{
    e.stopPropagation();
    setEditInfo(info);
    openEditDialog();

    //degug
    //console.log(info);
  };

  const handleUpdate = (info)=>{
    updateCracha({...info});
    closeEditDialog();
  };
  
  const handlePrint = ()=>{
    const ids = [];
    crachaList.forEach((craha)=>{
      if(selectedItens.current[craha.id]){
        ids.push(craha.id);
      }
    });
    window.open(`/${BASENAME}imprimir/cracha/?ids=${ids.join(',')}`, '', '');
  };
  
  const handleChange = (e)=>{
    query[e.target.id] = e.target.value;
    setQuery({...query});
    //degub
    //console.log(query)
  };

  useEffect(()=>{
    updateList('group', [query.ume, query.refType, query.refYear, query.refClass]);
    //debug
    //console.log('ok')
  // eslint-disable-next-line react-hooks/exhaustive-deps
  },[]);

  useEffect(()=>{
    updateList('group', [query.ume, query.refType, query.refYear, query.refClass]);
    //degub
    //console.log(query);
  // eslint-disable-next-line react-hooks/exhaustive-deps
  },[query]);

  return(
    <div className="h-full flex flex-col gorw">

      <div className="py-3 px-2 flex flex-row items-center gap-2 [&_label]:text-neutral-50">
        <label htmlFor="ume">Escola:</label>
        <select id="ume" onChange={handleChange}>
          <option>UME Monte Cabrão</option>
        </select>

        <label htmlFor="refType">Tipo:</label>
        <select id="refType" onChange={handleChange}>
          <option value={'regular'}>Transporte Regular</option>
          <option value={'escola-total'}>Escola Total</option>
        </select>

        <label htmlFor="refYear">Ano:</label>
        <input id="refYear" type="number" onBlur={handleChange}/>

        <label htmlFor="refClass">Sala:</label>
        <select onChange={handleChange} id="refClass">
          <option value={'MII-A'}>M II-A</option>
          <option value={'JD-A'}>JD-A</option>
          <option value={'PRE-A'}>PRÉ-A</option>
          <option value={'1-A'}>1º-A</option>
          <option value={'2-A'}>2º-A</option>
          <option value={'3-A'}>3º-A</option>
          <option value={'4-A'}>4º-A</option>
          <option value={'5-A'}>5º-A</option>
          <option value={'6-A'}>6º-A</option>
          <option value={'7-A'}>7º-A</option>
          <option value={'8-A'}>8º-A</option>
          <option value={'9-A'}>9º-A</option>
        </select>
      </div>

      <SelectableList className="grow">
        {crachaList.map((cracha)=>{
          return (
            <SelecableItem key={cracha.id} onSelect={(sel)=>{selectedItens.current[cracha.id] = sel}}>
              <div className="w-full flex flex-row justify-between group">
                <span className="font-semibold">{cracha.name}</span>
                <button className="primary-btn text-sm px-1 py-0 opacity-0 group-hover:opacity-100"
                  onClick={e => handleEdit(e, cracha)}
                >Editar</button>
              </div>
            </SelecableItem> 
          );
        })}
      </SelectableList>

      <div className="py-3 px-2 flex flex-row items-center gap-2 [&_button]:w-35">
        <button className="primary-btn" onClick={handlePrint}>Imprimir</button>
        <button className="cancel-btn ml-auto" onClick={handleDelete}>Excluir</button>
        <button className="sucess-btn" onClick={openCreateDialog}>Novo Crachá</button>
      </div>

      <CrachaInfoDialog id={"crate"} title={"NOVO CRACHÁ"} open={createFlag} onConfirm={haldleCreate} onClose={closeCreateDialog}/>
      <CrachaInfoDialog id={"edit"} title={"EDITAR CRACHÁ"} open={editFlag} info={editInfo} onConfirm={handleUpdate} onClose={closeEditDialog}/>
    </div>
  );
}

export default CrachaCreator;