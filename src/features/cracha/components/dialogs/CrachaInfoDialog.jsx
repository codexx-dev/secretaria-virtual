import { useEffect, useRef } from "react";
import EntryDialog from "../../../../components/dialogs/EntryDialog";
import { makeEmptCrachaInfo } from "../../stores/crachaStore";

function CrachaInfoDialog({open=true, id, title, info, onConfirm, onClose}){
  const crachaInfo = useRef(info || makeEmptCrachaInfo());
  const canvas = useRef(null);

  const handleChange = (e)=>{
    const id = e.target.id.split('-').pop();
    crachaInfo.current[id] = e.target.value;
    //degug
    //console.log(crachaInfo.current);
  };

  const handlePikFile = (e)=>{
    const file = e.target.files?.[0];
    if (!file) return;
    
    const ctx = canvas.current.getContext("2d");
    const img = new Image();
    const reader = new FileReader();

    reader.onload = () => {
      img.src = reader.result;
    };

    img.onload = () => {
      const w = 100;
      const h = img.height * (w / img.width);
      const y = (120 - h) / 2;
      ctx.drawImage(img, 0, y, w, h);

      canvas.current.toBlob((blob)=>{
        crachaInfo.current.photo = blob;
      }, "image/webp", 0.7);
    };

    reader.readAsDataURL(file);

    //degug
    //console.log(crachaInfo.current);
  };

  useEffect(()=>{
    if(open){
      if (!info) {
        crachaInfo.current = makeEmptCrachaInfo();
        return;
      }
    }else{
      crachaInfo.current = makeEmptCrachaInfo();
      return;
    }

    crachaInfo.current = info;

    const name = document.getElementById(id+"-name");
    //const photo = document.getElementById(id+"-photo");
    const tel = document.getElementById(id+"-tel");
    const mainParent = document.getElementById(id+"-mainParent");
    const parentList = document.getElementById(id+"-parentList");

    name.value = info.name;
    //photo.files.push(info.photo);??
    tel.value = info.tel;
    mainParent.value = info.mainParent;
    parentList.value = info.parentList;

    //degug
    //console.log(crachaInfo.current);
  // eslint-disable-next-line react-hooks/exhaustive-deps
  },[open]);

  return(
    <EntryDialog open={open} title={title} onClose={onClose} onConfirm={() => onConfirm(crachaInfo.current)}>
      <div className="px-12 grid grid-cols-[auto_1fr] items-center gap-1">

        <label htmlFor={id+"-ume"}>Escola:</label>
        <select id={id+"-ume"} onChange={handleChange} disabled>
          <option value={''}>Selecione...</option>
          <option value={'ume-mc'}>UME Monte Cabrão</option>
          <option value={''}>Inserir Novo Registro</option>
        </select>

        <label htmlFor={id+"-name"}>Nome:</label>
        <input id={id+"-name"} onBlur={handleChange}/>

        <label htmlFor={id+"-photo"}>Foto:</label>
        <div>
          <input id={id+"-photo"} type="file" onChange={handlePikFile}/>
          <canvas className="mx-auto" ref={canvas} width={100} height={120}/>
        </div>

        <label htmlFor={id+"-tel"}>Telefones.:</label>
        <input id={id+"-tel"} type="tel" onBlur={handleChange}/>

        <label htmlFor={id+"-mainParent"}>Responsável:</label>
        <input id={id+"-mainParent"} onBlur={handleChange}/>

        <label htmlFor={id+"-parentList"}>Autorizados:</label>
        <input id={id+"-parentList"} onBlur={handleChange}/>

      </div>
    </EntryDialog>
  );
}

export default CrachaInfoDialog;